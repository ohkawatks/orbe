#
# $Id: parse.y 623 2008-08-15 01:09:21Z takaya $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
#
# IDL Parser for Racc
# Author: Takeshi Ohkawa <ohkawa-takeshi@aist.go.jp>
#
class IdlParser

token ID

convert
 ID '"IDENTIFIER"'
end

rule

#1
specification: definition_s
{result=IdlSpecification.new("root")
    val[0].each do |v| result.addChild(v) end if val[0]!=nil
}

#2
definition: 
type_dcl ";"
| const_dcl ";"
| except_dcl ";" {result = val[0]}
| interface ";" {result = val[0]}
| module ";" {result = val[0]}
#| value ";"
#| type_id_dcl ";"

#2 for *
definition_s: | definition definition_s
{
  result = Array.new
    result.push(val[0])
    val[1].each do |v| result.push(v) end if val[1]!=nil
}
#2 for +
definition_p: definition definition_s
{
  result = Array.new
    result.push(val[0])
    val[1].each do |v| result.push(v) end if val[1]!=nil
}

#3
module: "module" identifier "{" definition_p "}"
{result = IdlModule.new(val[1])
    val[3].each do |v| result.addChild(v) end if val[3] != nil
}

#4
interface: interface_dcl 
{result = val[0]}

#5
interface_dcl: interface_header "{" interface_body "}"
{result = IdlInterface.new(val[0])
    val[2].each do |v| result.addChild(v) end
}

#6
forward_dcl: forward_dcl_1 "interface" identifier
#6-1
forward_dcl_1: | "abstract" | "local"

#7
interface_header: interface_header_1 "interface" identifier interface_header_2
{result = val[2]}
#7-1
interface_header_1: | "abstract" | "local"
#7-2
interface_header_2: | interface_inheritance_spec

#8
interface_body: export_s
{result = val[0]}

#9
export: op_dcl ";"
{result = val[0]}

#9 for *
export_s: | export export_s
{result = Array.new
    result.push(val[0])
    val[1].each do |v| result.push(v) end if val[1]!=nil
}

#10
interface_inheritance_spec:

#12
scoped_name: identifier 
| ":" ":" identifier 
{result="::"+val[2]}
| scoped_name ":" ":" identifier
{result=val[0]+"::"+val[3]}

#27
const_dcl: "const" const_type identifier "=" const_exp

#28
const_type: integer_type | char_type | wide_char_type | boolean_type | floating_pt_type
| string_type | wide_string_type | fixed_pt_const_type | scoped_name | octet_type

#29
const_exp: or_expr

#30
or_expr: xor_expr | or_expr "|" xor_expr

#31
xor_expr: and_expr | xor_expr "^" and_expr

#32
and_expr: shift_expr | and_expr "&" shift_expr

#33
shift_expr: add_expr | shift_expr ">>" add_expr | shift_expr "<<" add_expr

#34
add_expr:

#40
boolean_literal: "TRUE" | "FALSE"

#41
positive_int_const: const_exp

#42
type_dcl: "typedef" type_declarator 
| struct_type | union_type | enum_type | "native" simple_declarator | constr_forward_decl

#43
type_declarator: type_spec declarators

#44
type_spec: simple_type_spec | constr_type_spec

#45
simple_type_spec: base_type_spec | template_type_spec | scoped_name

#46
base_type_spec: floating_pt_type | integer_type | char_type | wide_char_type |
boolean_type | octet_type | any_type | object_type | value_base_type

#47
template_type_spec: sequence_type | string_type | wide_string_type | fixed_pt_type

#48
constr_type_spec: struct_type | union_type | enum_type

#49
declarators: declarator | declarators "," declarator

#50
declarator: simple_declarator | complex_declarator

#51
simple_declarator: identifier

#52 To be fixed
complex_declarator: identifier

#53
floating_pt_type: 
"float" {p 'float' if @debug}
| "double" {p 'double' if @debug}
| "long" "double" {p 'long double' if @debug}

#54
integer_type: signed_int | unsigned_int

#55
signed_int: signed_short_int | signed_long_int | signed_longlong_int

#56
signed_short_int: "short"

#57
signed_long_int: "long"

#58
signed_longlong_int: "longlong"

#59:
unsigned_int: unsigned_short_int | unsigned_long_int | unsigned_longlong_int

#60
unsigned_short_int: "unsigned" "short"
{result = "unsigned short"}

#61
unsigned_long_int: "unsigned" "long"
{result = "unsigned long"}

#62
unsigned_longlong_int: "unsigned" "longlong"
{result = "unsigned longlong"}

#63
char_type:"char"

#64
wide_char_type:"wchar"

#65
boolean_type:"boolean"

#66
octet_type:"octet"

#67
any_type:"any"

#68
object_type:"Object"

#69
struct_type: "struct" identifier "{" member_list "}"
{result=IdlStruct.new(val[1])
    val[3].each do |v| result.addChild(v) end if val[3]!=nil
}

#70
member_list: member 
{result=Array.new
    result.push(val[0])
}
| member member_list
{result=Array.new
    result.push(val[0])
    val[1].each do |v| result.push(v) end if val[1]!=nil
}

#71
member: type_spec declarators ";"
{result=IdlMember.new(val[1])
    result.setTypeSpec(val[0])
}

#71 for *
member_s: | member member_s
{result=Array.new
    result.push(val[0])
    val[1].each do |v| result.push(v) end if val[1]!=nil
}

#72
union_type: "union" identifier "switch" "(" switch_type_spec ")" "{" switch_body "}"

#73
switch_type_spec: integer_type | char_type | boolean_type | enum_type | scoped_name

#74
switch_body: case | case switch_body

#75
case: case_1 element_spec ";"
#75-1
case_1: case_label | case_label case_1

#76
case_label: "case" const_exp ":" | "default" ":"

#77
element_spec: type_spec declarator

#78
enum_type: "enum" identifier "{" enumerator enum_type_1 "}"
#78-1
enum_type_1: | "," enumerator

#79
enumerator: identifier

#80
sequence_type: "sequence" identifier

#81
string_type: "string"

#82
wide_string_type: "wstring"

#86
except_dcl: "exception" identifier "{" member_s "}" 
{result = IdlException.new(val[1])
    val[3].each do |v| result.addChild(v) end
}

#87
op_dcl: op_dcl_1 op_type_spec identifier parameter_dcls op_dcl_2 op_dcl_3
{result = IdlOperation.new(val[2])
   result.setAttribute(val[0])
   result.setTypeSpec(val[1])
   val[3].each do |v| result.addChild(v) end
   result.setRaises(val[4])
}
#87-1
op_dcl_1: | op_attribute 
#87-2
op_dcl_2: | raises_expr
#87-3
op_dcl_3: | context_expr

#88
op_attribute: "oneway"

#89
op_type_spec: param_type_spec | "void"

#90
parameter_dcls: "(" param_dcl parameter_dcls_1 ")" 
{result = Array.new
    result.push(val[1])
    val[2].each do |v| result.push(v) end if val[2]!=nil
}
| "(" ")"
{result = Array.new}

#90-1
parameter_dcls_1: | "," param_dcl parameter_dcls_1
{result = Array.new
    result.push(val[1])
    val[2].each do |v| result.push(v) end if val[2]!=nil
}

#91
param_dcl: param_attribute param_type_spec simple_declarator
{result = IdlParameter.new(val[2])
    result.setTypeSpec(val[1])
    result.setAttribute(val[0])
}

#92
param_attribute: "in" | "out" | "inout"

#93
raises_expr: "raises" "(" scoped_name raises_expr_1 ")"
{result = Array.new
    result.push(IdlException.new(val[2]))
    val[3].each do |v| result.push(v) end if val[3]!=nil
}
#93-1
raises_expr_1: | "," scoped_name raises_expr_1
{result = Array.new
    result.push(IdlException.new(val[1]))
    val[2].each do |v| result.push(v) end if val[2]!=nil
}

#94
context_expr: "context" "(" ")"

#95
param_type_spec: base_type_spec | string_type | wide_string_type | scoped_name

#96
fixed_pt_type: "fixed" "<" positive_int_const "," positive_int_const ">"

#97
fixed_pt_const_type: "fixed"

#98
value_base_type: "ValueBase"

#99
constr_forward_decl: "struct" identifier | "union" identifier

#???

identifier: ID
{print 'DEBUG - ID found: ', result, "\n" if @debug}


end
