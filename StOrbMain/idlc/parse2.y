#
# $Id: parse2.y 1011 2009-02-03 08:21:12Z takaya $
# Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology
#

class IDLParser

rule
#1
specification		: specification_1 specification_2
						{
							result = IDL::Specification.new(val[0], val[1])
						}

specification_1		:
						{
							result = []
						}
					| specification_1 import
						{
							result = val[0] << val[1]
						}

specification_2		: definition
						{
							result = [ val[0] ]
						}
					| specification_2 definition
						{
							result = val[0] << val[1]
						}

#2
definition			: type_dcl ";"
					| const_dcl ";"
					| except_dcl ";"
					| interface	";"
					| module ";"
					| value ";"
					| type_id_dcl ";"

#3
module				: MODULE identifier "{" module_1 "}"

module_1			: definition
					| module_1 definition

#4
interface			: interface_dcl
					| forward_dcl

#5
interface_dcl		: interface_header '{' interface_body '}'
						{
							result = IDL::Interface.new( val[0], val[2] )
						}

#6
forward_dcl			: forward_dcl_1 identifier

forward_dcl_1		:
					| ABSTRACT
					| LOCAL

#7
interface_header	: interface_header_1 INTERFACE identifier interface_header_2
						{
							result = IDL::InterfaceHeader.new( val[0], val[2], val[3] )
						}

interface_header_1	: forward_dcl_1				## reuse the rule. See forward_dcl_1 !!

interface_header_2	:
					| interface_inheritance_spec

#8
interface_body		:
						{
							result = []
						}
					| interface_body export
						{
							result = val[0] << val[1]
						}

#9
export				: type_dcl ';'
					| const_dcl ';'
					| except_dcl ';'
					| attr_dcl ';'
					| op_dcl ';'
					| type_id_dcl ';'
					| type_prefix_dcl ';'

#10
interface_inheritance_spec : ':' interface_name_1

interface_name_1	: interface_name
					| interface_name_1 "," interface_name

#11
interface_name		: scoped_name

#12
scoped_name			: identifier
						{
							result = val[0]
						}
					| '::' identifier
						{
							result = "_#{val}"
						}
					| scoped_name '::' identifier
						{
							result = "#{val[0].execute}_#{val[2]}"
						}
#13
value				: value_dcl
					| value_abs_dcl
					| value_box_dcl
					| value_forward_dcl

#14
value_forward_dcl	: VALUETYPE identifier
				 	| ABSTRACT VALUETYPE identifier 

#15
value_box_dcl		: VALUETYPE identifier type_spec

#16
value_abs_dcl		: ABSTRACT VALUETYPE identifier value_inheritance_spec '{' value_abs_dcl_1 '}'

value_abs_dcl_1		: interface_body		## reuse it

#17
value_dcl			: value_header '{' value_dcl_1 '}'

value_dcl_1			:
					| value_dcl_1 value_element

#18
value_header		: CUSTOM VALUETYPE identifier value_inheritance_spec
					|        VALUETYPE identifier value_inheritance_spec

#19
value_inheritance_spec : value_inheritance_spec_1 value_inheritance_spec_2

value_inheritance_spec_1 :
						 | ':' TRUNCATABLE value_inheritance_spaec_1_1
						 | ':' value_inheritance_spaec_1_1

value_inheritance_spaec_1_1 : value_name
						    | value_inheritance_spaec_1_1 "," value_name

value_inheritance_spec_2 :
						 | SUPPORTS value_inheritance_spaec_2_2

value_inheritance_spaec_2_2 : interface_name
							| value_inheritance_spaec_2_2 "," interface_name

#20
value_name			: scoped_name

#21
value_element		: export
					| state_member
					| init_dcl

#22
state_member		: PUBLIC type_spec declarators ";"
					| PRIVATE type_spec declarators ";"

#23
init_dcl			: FACTORY identifier "(" ")" ";"
					| FACTORY identifier "(" ")" raises_expr ";"
					| FACTORY identifier "(" init_param_dcls ")" ";"
					| FACTORY identifier "(" init_param_dcls ")" raises_expr ";"

#24
init_param_dcls		: init_param_dcl
					| init_param_dcls "," init_param_dcl

#25
init_param_dcl		: init_param_attribute param_type_spec simple_declarator

#26
init_param_attribute : IN

#27
const_dcl			: CONST const_type identifier "=" const_exp

#28
const_type			: integer_type
					| char_type
					| wide_char_type
					| boolean_type
					| floating_pt_type
					| string_type
					| wide_string_type
					| fixed_pt_const_type
					| scoped_name
					| octet_type

#29
const_exp			: or_expr

#30
or_expr				: xor_expr
					| or_expr "|" xor_expr

#31
xor_expr			: and_expr
					| xor_expr "^" and_expr

#32
and_expr			: shift_expr
					| and_expr "&" shift_expr

#33
shift_expr			: add_expr
					| shift_expr ">>" add_expr
					| shift_expr "<<" add_expr

#34
add_expr			: mult_expr
					| add_expr "+" mult_expr
					| add_expr "-" mult_expr

#35
mult_expr			: unary_expr
					| mult_expr "*" unary_expr
					| mult_expr "/" unary_expr
					| mult_expr "%" unary_expr

#36
unary_expr			: unary_operator primary_expr
					| primary_expr

#37
unary_operator		: "-" | "+" | "~" 
#38
primary_expr		: scoped_name
					| literal
					| "(" const_exp ")"

#39
literal				: INTEGER_LITERAL
					| STRING_LITERAL
					| WIDE_STRING_LITERAL
					| CHARACTER_LITERAL
					| WIDE_CHARACTER_LITERAL
					| FIXED_PT_LITERAL
					| FLOATING_PT_LITERAL
					| boolean_literal

#40
boolean_literal		: TRUE
					| FALSE

#41
positive_int_const	: const_exp

#42
type_dcl			: TYPEDEF type_declarator
						{
							result = IDL::TypedefDcl.new( val[1][0], val[1][1] )
						}
					| struct_type
					| union_type
					| enum_type
					| NATIVE simple_declarator
					| constr_forward_decl

#43
type_declarator		: type_spec declarators
						{
							result = val
						}

#44
type_spec			: simple_type_spec
					| constr_type_spec

#45
simple_type_spec	: base_type_spec
					| template_type_spec
					| scoped_name

#46
base_type_spec		: floating_pt_type
					| integer_type
					| char_type
					| wide_char_type
					| boolean_type
					| octet_type
					| any_type
					| object_type
					| value_base_type

#47
template_type_spec	: sequence_type
					| string_type
					| wide_string_type
					| fixed_pt_type

#48
constr_type_spec	: struct_type
					| union_type
					| enum_type

#49
declarators			: declarator
						{
							result = [ val[0] ]
						}
					| declarators "," declarator
						{
							result = val[0] << val[2]
						}

#50
declarator			: simple_declarator
					| complex_declarator

#51
simple_declarator	: identifier

#52
complex_declarator	: array_declarator
						{
							result = IDL::ArrayType.new(val[0][0], val[0][1])
						}

#53
floating_pt_type	: FLOAT
					| DOUBLE
					| LONG DOUBLE
						{
							result = "long double"
						}

#54
integer_type		: signed_int
					| unsigned_int

#55
signed_int			: signed_short_int
					| signed_long_int
					| signed_longlong_int

#56
signed_short_int	: SHORT

#57
signed_long_int		: LONG

#58
signed_longlong_int	: LONG LONG
						{
							result = "long long"
						}

#59
unsigned_int		: unsigned_short_int
					| unsigned_long_int
					| unsigned_longlong_int

#60
unsigned_short_int	: UNSIGNED SHORT
						{
							result = "unsigned short"
						}

#61
unsigned_long_int	: UNSIGNED LONG
						{
							result = "unsigned long"
						}

#62
unsigned_longlong_int : UNSIGNED LONG LONG
						{
							result = "unsigned long long"
						}

#63
char_type			: CHAR

#64
wide_char_type		: WCHART

#65
boolean_type		: BOOLEAN

#66
octet_type			: OCTET

#67
any_type			: ANY

#68
object_type			: OBJECT

#69
struct_type			: STRUCT identifier '{' member_list '}'
						{
							result = IDL::Struct.new( val[1], val[3] )
						}

#70
member_list			: member 
						{
							result = [ val[0] ]
						}
					| member_list member
						{
							result = val[0] << val[1]
						}

#71
member				: type_spec declarators ';'
						{
							result = IDL::Member.new( val[0], val[1] )
						}

#72
union_type			: UNION identifier SWITCH '(' switch_type_spec ')' '{' switch_body '}'
						{
							result = IDL::Union.new( val[1], val[4], val[7] )
						}

#73
switch_type_spec	: integer_type
					| char_type
					| boolean_type
					| enum_type
					| scoped_name

#74
switch_body			: case
						{
							result = [ val[0] ]
						}
					| switch_body case
						{
							result = val[0] << val[1]
						}
#75
case				: case_1 element_spec ';'
						{
							result = IDL::Case.new( val[0].flatten, val[1] )
						}

case_1				: case_label
						{
							result = [ val[0] ]
						}
					| case_1 case_label
						{
							result = val[0] << val[1]
						}

#76
case_label			: CASE const_exp ":"
						{
							result = IDL::CaseLabel.new( val[1] );
						}
					| DEFAULT ":"
						{
							result = IDL::DefaultLabel.new( val[0] );
						}

#77
element_spec		: type_spec declarator
						{
							result = IDL::ElementSpec.new( val[0], val[1] );
						}

#78
enum_type			: ENUM identifier "{" enum_type_1 "}"

enum_type_1			: enumerator
						{
							result = val
						}
					| enum_type_1 "," enumerator
						{
							result = val[0] << val[1]
						}
#79
enumerator			: identifier

#80
sequence_type		: SEQUENCE "<"  simple_type_spec "," positive_int_const ">" 
						{
							result = IDL::Sequence.new( val[2], val[4] )
						}
					| SEQUENCE "<" simple_type_spec ">"
						{
							result = IDL::Sequence.new( val[2] )
						}
			
#81
string_type			: STRING "<" positive_int_const ">"
					| STRING

#82
wide_string_type	: WSTRING "<" positive_int_const ">"
					| WSTRING

#83
array_declarator	: identifier array_declarator_1
						{
							result = val
						}

array_declarator_1	: '[' positive_int_const ']'
						{
							result = [ val[1] ]
						}
					| array_declarator_1 '[' positive_int_const ']'
						{
							result = val[0] << val[2]
						}

#85
attr_dcl			: readonly_attr_spec
					| attr_spec

#86
except_dcl			: EXCEPTION identifier '{' except_dcl_1 '}'
						{
							result = IDL::Exception.new( val[1], val[3] )
						}

except_dcl_1		:
						{
							result = []
						}
					| except_dcl_1 member
						{
							result = val[0] << val[1]
						}

#87
op_dcl				: op_dcl_1 op_type_spec identifier parameter_dcls op_dcl_2 op_dcl_3
						{
							result = IDL::Operation.new( val[0], val[1], val[2], val[3], val[4], val[5] )
						}

op_dcl_1			:
					| op_attribute

op_dcl_2			:
					| raises_expr

op_dcl_3			:
					| context_expr

#88
op_attribute		: ONEWAY

#89
op_type_spec		: param_type_spec
					| VOID

#90
parameter_dcls		: "(" parameter_dcls_1 ")" 
						{
							result = IDL::ParameterDcls.new(val[1])
						}

parameter_dcls_1	:
					| param_dcl
						{
							result = val
						}
					| parameter_dcls_1 "," param_dcl 
						{
							result = val[0] << val[2]
						}

#91
param_dcl			: param_attribute param_type_spec simple_declarator
						{
							result = IDL::Parameter.new( val[0], val[1], val[2] ) ## parameter
						}

#92
param_attribute		: IN | OUT | INOUT

#93
raises_expr			: RAISES "(" raises_expr_1 ")"

raises_expr_1		: scoped_name
					| raises_expr_1 "," scoped_name 

#94
context_expr		: CONTEXT "(" context_expr_1 ")"

context_expr_1		: STRING_LITERAL
					| context_expr_1 "," STRING_LITERAL

#95
param_type_spec		: base_type_spec
					| string_type
					| wide_string_type
					| scoped_name

#96
fixed_pt_type		: FIXED "<" positive_int_const "," positive_int_const ">"

#97
fixed_pt_const_type	: FIXED

#98
value_base_type		: VALUEBASE

#99
constr_forward_decl	: STRUCT identifier
					| UNION identifier

#100
import				: IMPORT imported_scope ";"

#101
imported_scope		: scoped_name
					| STRING_LITERAL

#102
type_id_dcl			: TYPEID scoped_name STRING_LITERAL

#103
type_prefix_dcl		: TYPEPREFIX scoped_name STRING_LITERAL

#104
readonly_attr_spec	: READONLY ATTRIBUTE param_type_spec readonly_attr_declarator

#105
readonly_attr_declarator : simple_declarator rases_expr
						 | readonly_attr_declarator_1

readonly_attr_declarator_1 : simple_declarator
						   | readonly_attr_declarator_1 "," simple_declarator

#106
attr_spec			: ATTRIBUTE param_type_spec attr_declarator

#107
attr_declarator		: simple_declarator attr_raises_expr
					| attr_declarator_1

attr_declarator_1	: simple_declarator
					| attr_declarator_1 "," simple_declarator

#108
attr_raises_expr	: get_excep_expr
					| get_excep_expr set_excep_expr
					| set_excep_expr

#109
get_excep_expr		: GETRAISES exception_list

#110
set_excep_expr		: SETRAISES exception_list

#111
exception_list		: "(" exception_list_1 ")"

exception_list_1	: scoped_name
					| exception_list_1 "," scoped_name

end
---- header

require "strscan"
require "marshall"
require "idl"
require "template"
require "pp"

---- inner

##	include	IDL::Template

	RESERVED = {
		"abstract"		=> :ABSTRACT,
		"any"			=> :ANY,
		"attribute"		=> :ATTRIBUTE,
		"boolean"		=> :BOOLEAN,
		"case"			=> :CASE,
		"char"			=> :CHAR,
		"component"		=> :COMPONENT,
		"const"			=> :CONST,
		"consumes"		=> :CONSUMES,
		"context"		=> :CONTEXT,
		"custom"		=> :CUSTOM,
		"default"		=> :DEFAULT,
		"double"		=> :DOUBLE,
		"exception"		=> :EXCEPTION,
		"emits"			=> :EMITS,
		"enum"			=> :ENUM,
		"eventtype"		=> :EVENTTYPE,
		"factotry"		=> :FACTORY,
		"FALSE"			=> :FALSE,
		"finder"		=> :FINEDER,
		"fixed"			=> :FIXED,
		"float"			=> :FLOAT,
		"getraises"		=> :GETRAISES,
		"home"			=> :HOME,
		"import"		=> :IMPORT,
		"in"			=> :IN,
		"inout"			=> :INOUT,
		"interface"		=> :INTERFACE,
		"local"			=> :LOCAL,
		"long"			=> :LONG,
		"module"		=> :MODULE,
		"multiple"		=> :MULTIPLE,
		"native"		=> :NATIVE,
		"Object"		=> :OBJECT,
		"octet"			=> :OCTET,
		"oneway"		=> :ONEWAY,
		"out"			=> :OUT,
		"primarykey"	=> :PRIMARYKEY,
		"private"		=> :PRIVATE,
		"provides"		=> :PROVIDES,
		"public"		=> :PUBLIC,
		"publishes"		=> :PUBLISHES,
		"raises"		=> :RAISES,
		"readonly"		=> :READONLY,
		"setraises"		=> :SETRAISES,
		"sequence"		=> :SEQUENCE,
		"short"			=> :SHORT,
		"string"		=> :STRING,
		"struct"		=> :STRUCT,
		"supports"		=> :SUPPORTS,
		"switch"		=> :SWITCH,
		"TRUE"			=> :TRUE,
		"truncatable"	=> :TRUNCATABLE,
		"typedef"		=> :TYPEDEF,
		"typeid"		=> :TYPEID,
		"typeprefix"	=> :TYPEPREFIX,
		"unsigned"		=> :UNSIGNED,
		"union"			=> :UNION,
		"uses"			=> :USES,
		"ValueBase"		=> :VALUEBASE,
		"valuetype"		=> :VALUETYPE,
		"void"			=> :VOID,
		"wchar"			=> :WCHAR,
		"wstring"		=> :WSTRING,
		"unsigned short" => :USHORT,
		"unsigned long"  => :ULONG,
		"unsigned long long"  => :ULONGLONG,
		"long long"     => :LONGLONG,
		"long double"   => :LONGDOUBLE,
	}

	def initialize()
		@yydebug = true
		@q = []
	end

	def parseFile( f )
		f.each do |line|
			parse(line)
		end
		@q.push [false, '$']

		tree = do_parse()

		tree.dump if ENV['DEBUG_IDL'].to_i > 1

		context = IDL::Context.new()

		[ tree.execute( context ), tree.generate( context ) ]
	end

	def parse(str)
		s = StringScanner.new(str)
		until s.eos?
			s.scan(/\A\s+/)					## whitespace
			s.scan(/#.*$/)					## # something

			s.scan(/::/)					## '::'
			if s[0]
				@q.push [s[0], s[0]]	
			end

			s.scan(/[\[\]{};<>(),=:]/)		## particular symbols
			if s[0]
				@q.push [s[0], s[0]]	
			end

			s.scan(/"[^"]*"/)				## string
			if s[0]
				@q.push [:STRING_LITERAL, s[0]]	
			end

			s.scan(/\A\d+/)					## integer
			if s[0]
				@q.push [:INTEGER_LITERAL, s[0]]
			end

			s.scan(/\w+/)
			if s[0]
				if RESERVED.key?(s[0])
					@q.push [RESERVED[s[0]], s[0]]
				else 
					@q.push [:identifier, s[0]]
				end
			end
		end
	end

	def next_token
		@q.shift
	end

	attr_reader	:definitions

---- footer


if $0 == __FILE__

if ARGV[0] && File.file?(ARGV[0])
	raise "file suffix must be \"idl\"!" unless (ARGV[0] =~ /\.idl$/)

	f = IDL::File.new( ARGV[0] )

	s = [ "", "" ]
	open("|cpp #{f.file}") do |idl|
		tmp = IDLParser.new.parseFile( idl );
		s[0] += tmp[0]
		s[1] += tmp[1]
	end

	# header
	out = (ENV['DEBUG_IDL'].to_i > 0) ? $stdout : File.open(f.header, "w")
	out.puts( "--- header ---" ) if ENV['DEBUG_IDL'].to_i > 0
	out.puts( sprintf( ::IDL::HEADER.gsub(/\$FILE\$/, f.header).
						gsub(/\$INPUT\$/, f.file).
						gsub(/\$TIME\$/, Time.now.to_s).
						gsub(/\$MACRO\$/, f.macro), s[0]) )

	# impl (only when it's running as no-debug mode)
	unless ENV['DEBUG_IDL']. to_i > 0
		out = File.open(f.impl, "w")
		out.puts( sprintf( ::IDL::IMPL.gsub(/\$FILE\$/, f.impl).
						gsub(/\$INPUT\$/, f.file).
						gsub(/\$TIME\$/, Time.now.to_s).
						gsub(/\$MACRO\$/, f.macro).
						gsub(/\$INCLUDE\$/, File::basename( f.header )),
							s[0][(s[0] =~ /extern/) .. -1].
								gsub(/extern /, "").
								gsub(/;\n/, "\n{\n\t/* need implementation */\n}\n\n")))
	end

	# body (dispatch)
	out = (ENV['DEBUG_IDL']. to_i > 0) ? $stdout : File.open(f.body, "w") 
	out.puts( "--- body ---" ) if ENV['DEBUG_IDL'].to_i > 0
	out.puts( sprintf( ::IDL::BODY.gsub(/\$FILE\$/, f.body).
						gsub(/\$INPUT\$/, f.file).
						gsub(/\$TIME\$/, Time.now.to_s).
						gsub(/\$MACRO\$/, f.macro).
						gsub(/\$INCLUDE\$/, File::basename( f.header )), s[1]) )
	exit 

end
end
