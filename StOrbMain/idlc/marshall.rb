#
# $Id: marshall.rb 1014 2009-02-03 09:45:21Z takaya $
# Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology 
#
module IDL
module Marshall

class Marshall
	@@in = {
		1 => "%s = *req_ptr;\n",
		2 => "%s = Orbe_exchangeShort(*(u16*)req_ptr);\n",
		4 => "%s = Orbe_exchangeLong(*(u32*)req_ptr);\n",
		8 => "%s = Orbe_exchangeDouble(*(CORBA_double*)req_ptr);\n",
	}

	@@out = {
		1 => "*reply_ptr = %s;\n",
		2 => "memcpy(reply_ptr, (u8*)&%s, 2);\n",
		4 => "memcpy(reply_ptr, (u8*)&%s, 4);\n",
		8 => "memcpy(reply_ptr, (u8*)&%s, 8);\n",
	}
	@@out.default = "memcpy(reply_ptr, (u8*)&%s, sizeof(%s));\n"

	def initialize(ident, size)
		@ident = ident
		@size = size
	end

	def execute( context, arg )
		arg.attribute =~ /out/    and return "#{@ident} *#{arg.vname}"
		arg.attribute =~ /return/ and return "#{@ident} "
		"#{@ident} #{arg.vname}"
	end

	def to_s
		@ident
	end

	def as_arg( context, arg )
		arg.attribute =~ /out/ and return "&#{arg.vname}"
		arg.vname
	end


	def decl( context, arg )
		"#{@ident} #{arg.vname};\n"
	end

	def init( context, arg, toward=true )
		s = ""
		if arg.attribute =~ /in/
			s = sprintf(@@in[@size], arg.vname)
			if (toward)
				s += "req_ptr += sizeof(#{@ident});\n"
			end
		end
		s
	end

	def post( context, arg, toward=true )
		s = ""
		if arg.attribute =~ /out|return/
			s = sprintf(@@out[@size], arg.vname, arg.vname)
			if (toward)
				s += "reply_ptr += sizeof(#{@ident});\n"
			end
		end	
		s
	end

	def as_return( context, arg)
		to_s + " " + arg.vname
	end

	def pointer
		""
	end
end

# 1byte
class Octet     <Marshall; end
class Char      <Octet;    end
class Boolean   <Octet;    end

class Short     <Marshall; end
class UShort    <Short;    end

# 4bytes
class Long      <Marshall; end
class ULong     <Long;     end
class Float     <Long;     end
class TypeCode  <Long;     end

# 8bytes
class Double    <Marshall
	def post( context, arg, toward=true )
		s = ""
		if arg.attribute =~ /out|return/
			s = "#{arg.vname} = swapDoubleOutput(#{arg.vname});\n"
			s += super(context, arg, toward)
		end
		s
	end
end
class LongLong  <Marshall; end
class ULongLong <Marshall; end

#
class String    <Marshall
	def init(context, arg)
		if arg.attribute =~ /in/
			return "Orbe_getString(#{arg.vname}, req_ptr);\n"
		end
		""
	end

	def post(context, arg)
		if arg.attribute =~ /out|return/
			return "Orbe_putString(#{arg.vname}, reply_ptr);\n"
		end
		""
	end
end

class Struct	<Marshall
	def initialize(ident, type)
		@ident = ident
		@type = type
	end

	def execute( context, arg )
		arg.attribute =~ /return/ and return @ident.execute( context ) + " "
		"#{@ident.execute( context )} *#{arg.vname}"
	end

	def as_arg( context, arg )
		"&#{arg.vname}"
	end

	def as_return( context, arg )
		@ident.execute( context ) + " " + arg.vname
	end

	def init(context, arg)
		s = ""
		if arg.attribute =~ /in/
			@type.members.each do |member|
				member.dcls.each do |dcl|
					s += context.marshall(member.type).init( context,
							Parameter.new(arg.attribute, member.type, "#{arg.vname}.#{dcl}"))
				end
			end
		end
		s
	end
end

class Union     <Struct
	def init(context, arg)
		s = ""
		if arg.attribute =~ /in/
			d = context.marshall(@type._d_type).init( context,
						Parameter.new(arg.attribute, @type._d_type, "#{arg.vname}._d"))
			s0 = ""
			for member in @type.members
				for label in member.children[0]
					s0 += label.execute( context )
				end
				s0 += context.marshall(member.children[1].type).init(
						context, Parameter.new(arg.attribute, member.children[1].type, "#{arg.vname}._u.#{member.children[1].dcl}"), false).chop
				s0 += " break;\n"
			end
			s = <<EOF
#{d}
switch(#{arg.vname}._d) {
#{s0}}
req_ptr += sizeof(#{arg.vname}._u);
EOF
		end
		s
	end
end

class WString   <Marshall; end
class LongDouble<Marshall; end
class Void      <Marshall
	def as_result( context, arg )
		""
	end

	def post( context, arg )
		""
	end
end

class WChar     <Marshall; end
class Enum      <Marshall; end

class Any       <Marshall
	def execute( context, arg )
		arg.attribute =~ /return/ and return "#{@ident} *"
		"#{@ident} *#{arg.vname}"
	end

	def init( context, arg )
		arg.attribute =~/in/ and return "Orbe_getAny(#{arg.vname}, req_ptr);\n"
		""
	end

	def as_arg( context, arg ) 
		"&#{arg.vname}"
	end

	def as_result( context, arg )
		"#{@ident} #{arg.vname}"
	end

	def post( context, arg )
		arg.attribute =~ /out|return/ and return "Orbe_putAny(#{arg.vname}, reply_ptr);\n"
		""
	end

	def pointer
		"*"
	end
end

class Sequence  <Struct;   end

class Array     <Marshall
	def initialize(ident, type, dim)
		@ident = ident
		@type = type	# each item's type
		@dim = dim		# dimentions
		@size = 1;
		for num in @dim
			@size *= num.to_i	# to calc tatal size
		end
	end

	def execute( context, arg )
		arg.attribute =~ /return/ and return @ident.execute( context ) + "_slice *"
		"#{@ident.execute( context )} #{arg.vname}"
	end

	def decl( context, arg )
		"#{@ident.execute( context )} *#{arg.vname};\n"
	end

	def as_arg( context, arg )
		"*#{arg.vname}"
	end

	def init(context, arg)
		s1 = "{\nsize_t i;\n#{arg.vname} = alloca(sizeof(#{@type.execute( context )}) * #{@size});\n"
		s2 = ""
		s3 = "}\n"
		if arg.attribute =~ /in/
			s2 += "#{@type.execute( context )} *p = (#{@type.execute( context )} *)#{arg.vname};\n"
			s2 += "for (i = 0; i < #{@size}; i++) {\n"
			s2 += context.marshall(@type).init( context, Parameter.new( arg.attribute, @type, "*p++") )
			s2 += "}\n"
		end
		s1 + s2 + s3
	end

	def as_return( context, arg )
		@ident.execute( context ) + "_slice *" + arg.vname
	end

	def post( context, arg )
		s = ""
		if arg.attribute =~ /out|return/ 
			s += "memcpy(reply_ptr, #{arg.vname}, sizeof(#{@ident.execute( context )}));\n"
			s += "reply_ptr += sizeof(#{@ident.execute( context )});\n"
		end
		s
	end
end

end ## Marshall
end ## IDL
