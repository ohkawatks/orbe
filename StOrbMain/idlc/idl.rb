#
# $Id: idl.rb 1019 2009-02-04 00:19:38Z takaya $
# Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology
#

class String
	def execute( context )
		if self ==  ";"
			return ";\n"
		elsif context.frame.reserved?(self)
			return context.frame.marshall(self).to_s
		else
			s = context.scope(self)
			s != "" and return "#{s}_#{self}"
		end
		self
	end

	def ident
		self
	end
end

class Array
	def execute(*args)
		s = ""
		for item in self
			s += item.execute(*args) if (item)
		end
		s
	end

	def generate(*args)
		s = ""
		for item in self 
			s += item.generate(*args) if (item)
		end
		s
	end
end

module IDL	## start

class File
	def initialize( f )
		@file = f
	end
	attr_reader :file

	def header
		::File.basename(@file, ".idl") + ".h"
	end

	def body
		::File.basename(@file, ".idl") + ".c"
	end

	def impl
		::File.basename(@file, ".idl") + "Impl.c"
	end

	def macro
		"_#{ ::File.basename(@file, ".idl").upcase }_H_"
	end
end

class Context
	def initialize
		@stack = [] 
		@stack.push( TypeMap.new( "" ) )
		@index = 0
	end

	def frame
		@stack[@index]
	end

	def global
		@stack[0]	## global scope
	end

	def push( typemap )
		@stack.push typemap
		@index += 1
	end

	def pop
		@index -= 1
	end

	def godown
		@index += 1
	end

	def comeup
		@index -= 1
	end

	def marshall( type )
		find_frame( type ).marshall( type )
	end

	def is_array?( type )
		marshall( type ).is_a?(Marshall::Array)
	end

	def find_frame( type )
		@stack[0].reserved?( type ) and return @stack[0]	## primitive types
		for frame in @stack.reverse
			if frame.key?( type.intern )
				return frame
			end
		end
		raise "unkown datatype : #{type}"
	end

	def scope( word=nil )
		if ( word )
			return find_frame( word ).scope
		end
		return frame.scope
	end
end

class TypeMap
	@@type_map = {
	    ## primitive symbol => Marshalls
		:VOID               => Marshall::Void.new("void", 0),
		:SHORT              => Marshall::Short.new("CORBA_short", 2),
		:LONG               => Marshall::Long.new("CORBA_long", 4),
		:LONGLONG           => Marshall::LongLong.new("CORBA_long", 4),
		:USHORT             => Marshall::UShort.new("CORBA_unsigned_short", 2),
		:ULONG              => Marshall::ULong.new("CORBA_unsigned_long", 4),
		:ULONGLONG          => Marshall::ULongLong.new("CORBA_unsigned_long_long", 8),
		:FLOAT              => Marshall::Float.new("CORBA_float", 4),
		:DOUBLE             => Marshall::Double.new("CORBA_double", 8),
		:LONGDOUBLE         => Marshall::LongDouble.new("CORBA_long_double", 8),
		:BOOLEAN            => Marshall::Boolean.new("CORBA_boolean", 1),
		:CHAR               => Marshall::Char.new("CORBA_char", 1),
		:WCHAR              => Marshall::WChar.new("CORBA_wchar", 2),
		:OCTET              => Marshall::Octet.new("CORBA_octet", 1),
		:ENUM               => Marshall::Enum.new("CORBA_enum", -1),
		:STRING             => Marshall::String.new("CORBA_char*", -1),
		:WSTRING            => Marshall::WString.new("CORBA_wchar*", -1),
		:ANY                => Marshall::Any.new("CORBA_any", 8),
		:TYPECODE           => Marshall::TypeCode.new("CORBA_TypeCode", 4),
	}

	def initialize(scope)
		@scope = scope
		@ltypes = {}
	end

	def key?(var)
		@ltypes.key?(var)
	end

	def reserved?(type)
		::IDLParser::RESERVED.key?(type) && @@type_map[::IDLParser::RESERVED[type]]
	end

	def marshall(type)
		@ltypes.key?(type.intern) && ( return @ltypes[type.intern] )
		reserved?(type)           && ( return @@type_map[::IDLParser::RESERVED[type]] )

		raise "unknown data type : #{type.to_s}"
	end

	def regist(dcl, type, dim=nil)
		if (type.is_a?(IDL::Union))
			@ltypes[dcl.intern] = Marshall::Union.new(dcl, type) and return
		elsif (type.is_a?(IDL::Struct))
			@ltypes[dcl.intern] = Marshall::Struct.new(dcl, type) and return
		elsif (type.is_a?(IDL::Sequence))
			@ltypes[dcl.intern] = Marshall::Sequence.new(dcl, type) and return
		else
			if (!dim)
				@ltypes.key?(type.intern)        and @ltypes[dcl.intern] = @ltypes[type.intern] and return
				::IDLParser::RESERVED.key?(type) and @ltypes[dcl.intern] = @@type_map[::IDLParser::RESERVED[type]] and return
			else
				@ltypes[dcl.intern] = Marshall::Array.new(dcl, type, dim)
			end
		end
#		raise "warning:: undefined type. #{type}, #{dcl}"
	end

	attr_reader :scope
end

##
## IDL components
##
class Element
	def initialize( *var )
		@children = var
	end

	def execute( context )
		s = ""
		@children && @children.each do |child|
			s += child.execute( context )	
		end
		s
	end

	def generate( arg )
		s = ""
		@children && @children.each do |child|
			s += child.generate( context )	
		end
		s
	end

	attr_accessor :ident, :children
end

class Struct <Element
	def initialize(ident, members)
		@ident = ident
		@members = members
	end

	attr_reader :members

	def execute( context )

		context.frame.regist(@ident, self)

		s = ""
		@members.each do |member|
			s += member.execute( context )
		end

		s =<<EOF
typedef struct #{@ident.execute( context )} {
#{s}} #{@ident.execute( context )};
EOF
	end

	def generate( context )
		""
	end

	def to_s
		@ident
	end
end

class Exception <Struct; end

class Specification <Element
	def initialize(import, definition)
		@import = import
		@definition = definition
	end

	def execute( context )
		s = ""
		for child in @definition
			s0 = child.execute( context )
			s += "#{s0}\n" if s0 != ""
		end
		s
	end

	def generate( context )
		s = ""
		for child in @definition
			s += child.generate( context )
		end
		s =<<EOF

void dispatch(Req *req, Reply *reply, Giop_Hdr *header)
{
    u8      *req_ptr;
    u8      *reply_ptr;
    u8      *start;

    req_ptr = req->body;
    reply_ptr = start = reply->body;

#{s}
    reply->body_size = (size_t) (reply_ptr - start);
}
EOF
	s
	end

	def dump
		puts "DUMP... for debug"
		pp @import
		pp @definition
	end
end

class Member <Element
	def initialize(type, dcls)
		@type = type
		@dcls = dcls
	end

	def execute( context )
		s = ""
		@dcls.each do |dcl|
			s += "#{@type.execute( context )} #{dcl.to_s};\n"
		end
		s
	end

	attr_reader :type, :dcls 
end

class Union <Struct
	def initialize(ident, _d_type, members)
		@ident   = ident
		@_d_type = _d_type
		@members = members
	end

	attr_reader :_d_type

	def execute( context )

		context.frame.regist(@ident, self)

		s = ""
		for child in @members
			s += child.execute( context )
		end
		s=<<EOF
typedef struct #{@ident.execute( context )} {
#{@_d_type.execute( context )} _d;
union {
#{s}} _u;
} #{@ident.execute( context )};
EOF
	end
end

class Case <Element; end

class CaseLabel <Element
	def initialize( label )
		@label = label
	end

	def execute( context )
		"case #{@label} : \n"
	end
end

class DefaultLabel <CaseLabel
	def execute( context )
		"default : \n"
	end
end

class ElementSpec <Element
	def initialize(type, dcl)
		@type = type
		@dcl = dcl
	end

	def execute( context )
		"#{@type.execute( context )} #{@dcl};\n"
	end

	attr_reader :type, :dcl
end

class Sequence <Element
	def initialize(type, max)
		@type = type
		@max  = max if (max && max.size > 1)
	end

	def execute( context )
		s =<<EOF
struct {
CORBA_unsigned_long _maximum;
CORBA_unsigned_long _length;
#{@type.execute( context )} *_buffer;
}
EOF
		s.chop!
	end
end

class ArrayType <Element
	def initialize(ident, dim)
		@ident = ident
		@dim = dim
	end

	def execute
		raise "callit"
		"#{@ident}[#{@dim.join("][")}]"
	end

	def to_s
		@ident
	end

	attr_reader :dim
end

class TypedefDcl <Element
	def initialize(type, dcls)
		@dcls = dcls
		@type = type
	end

	def execute( context )
		s = ""
		if @type.is_a?(Struct)  || @type.is_a?(Union) ## || @type.is_a?(Enum)
			s += @type.execute( context )
		end

		for dcl in @dcls
			dim = nil  

			if dcl.is_a?(ArrayType)
				dim = dcl.dim
				dcl = dcl.ident
			end

			context.frame.regist(dcl, @type, dim)

			s += "typedef "
			if @type.is_a?(Struct)  || @type.is_a?(Union) ## || @type.is_a?(Enum)
				s += @type.ident.execute( context ) 
			else
				s += @type.execute( context )
			end

			s += " #{dcl.execute( context )}"
			if (dim)
				s += "[#{ dim.join("][") }];\n"
				s += "typedef "
				if @type.is_a?(Struct)  || @type.is_a?(Union) ## || @type.is_a?(Enum)
					s += @type.ident.execute( context ) 
				else
					s += @type.execute( context )
				end
				s += " " + dcl.execute( context ) + "_slice"
				for i in dim[1..-1]
					s += "[" + i.to_s + "]" 
				end
			end
			s += ";\n"
		end
		s
	end

	def generate( context )
		""
	end
end

class InterfaceHeader <Element
	def initialize(forward, ident, inherit)
		@forward = forward
		@ident   = ident
		@inherit = inherit
	end

	def execute
		"typedef CORBA_Object #{@ident};\n"
	end
end

class Interface <Element
	def initialize(header, body)
		@header = header
		@body = body
	end

	def execute( context )
		s = @header.execute
		context.push( TypeMap.new( @header.ident ) )
		for dcl in @body
			s += dcl.execute( context )
		end
		context.pop
		s
	end

	def generate( context )
		s = ""
		context.godown
		@body.each do |dcl|
			s += dcl.generate( context )
		end
		context.comeup
		s
	end
end

class Operation <Element
	def initialize(oneway, type, ident, params, *var)
		@oneway = true if (oneway)
		@result = IDL::Parameter.new("return", type, "result")
		@ident = ident
		@params = params
		super(*var)
	end

	def execute( context )
		s = "extern "
		s += @result.execute( context )
		s += context.scope + "_" + @ident
		s += "(#{context.scope} o, #{@params.execute( context )} CORBA_Environment *ev);\n"
	end

	def generate( context )
		decl, init, body, post = @params.generate

		s = ""
		s += decl + "\n"
		s += init + "\n" 
		s += (@result.type.intern != :VOID) ? @result.as_return + " = " : ""
		s += "#{@result.pointer}#{context.scope}_#{@ident}(NULL, #{body} NULL);\n\n"
		s += (@result.type.intern != :VOID) ? @result.post : ""
		s += post + "\n"

		s = <<EOF
if (strcmp((void*)req->operation, "#{@ident}") == 0) {
	#{s}}
EOF
	end

	def to_s
		@ident
	end
end

class ParameterDcls <Element
	def initialize(params)
		@params = params
	end

	def execute( context )
		s = ""
		@params && @params.each do |param|
			s += param.execute( context ) + ", "
		end
		s
	end	

	def generate
		dcl = ""
		init = ""
		body = ""
		post = ""
		@params && @params.each do |param|
			tmp = param.as_arg
			dcl   += tmp[0]
			init  += tmp[1]
			body  += tmp[2] + ", "
			post  += tmp[3]
		end
		[ dcl, init, body, post ]
	end
end

class Parameter <Element
	def initialize(attribute, type, vname)
		@attribute = attribute
		@vname = vname
		@type = type
	end

	attr_reader :attribute, :type, :vname

	def execute( context )
		@context = context
		marshall.execute( @context, self )
	end

	def as_return
		marshall.as_return( @context, self )
	end

	def post
		marshall.post( @context, self)
	end

	def as_arg
		[ marshall.decl( @context, self )	|| "",
		  marshall.init( @context, self )	|| "",
		  marshall.as_arg( @context, self )	|| "",
		  marshall.post( @context, self ) 	|| "" ]
	end

	def pointer
		marshall.pointer
	end

	private

	def marshall
		@context.marshall( @type )
	end
end

end	## end for module IDL
