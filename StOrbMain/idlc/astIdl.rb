#
# $Id: astIdl.rb 796 2008-09-17 03:12:34Z takaya $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
#
# Astract Syntax Tree for IDL
#
class IdlElement
  def initialize(name)
    @name = name
    @children = Array.new
  end

  def setParent(parent)
    @parent=parent
  end

  def getParent
    return @parent
  end

  def getName
    return @name
  end

  def addChild(child)
    @children.push(child)
  end


  def setupConnection
    @children.each do |child| 
      child.setParent(self) 
      child.setupConnection
    end
  end

  def generateHeader
    @children.each do |child| child.generateHeader end
  end

  def generateDispatch
    @children.each do |child| child.generateDispatch end
  end

  def generateMarshaller
    @children.each do |child| child.generateMarshaller end
  end

  def generateTemplate
    @children.each do |child| child.generateTemplate end
  end

  def getMessage
    @parent.getMessage
  end

  def generateIncludeHeaders
    @parent.generateIncludeHeaders
  end

  def printHeaderInclude
    @children.each do |c| c.printHeaderInclude end
  end    

end

class IdlSpecification < IdlElement
  def setMessage(msg)
    @msg = msg
  end

  def getMessage
    return @msg
  end

  def generateIncludeHeaders
    @children.each do |c| c.printHeaderInclude end
  end    
end

class IdlModule < IdlElement
end

class IdlInterface < IdlElement
  def printHeaderInclude
    print "#include        \"" + getName + ".h\"\n"
  end

  def generateHeader
    filename = getName+".h"
    $stdout = File.open(filename, "w")
    filenameSymbol = "_" + getName.upcase + "_H"

    print  "// " + filename + "\n" + getMessage +
      "#ifndef " + filenameSymbol + "\n\n" +
      "#define " + filenameSymbol + "\n\n" +
      "#include        \"CORBA_Object.h\"\n" +
      "typedef CORBA_Object " + getName + ";\n\n"

    generateIncludeHeaders


    @children.each do |child| child.generateHeader end
    print "#endif //" + filenameSymbol + "\n"
    
    $stdout = STDOUT
  end

  def generateTemplate
    filename = getName+"Impl.c.template"
    $stdout = File.open(filename, "w")

    print  "// " + filename + "\n" + getMessage +
      "#include        \"CORBA_Object.h\"\n"
    generateIncludeHeaders
    print "\n\n"

    @children.each do |child| child.generateTemplate end

    $stdout = STDOUT
  end

  def generateDispatch
    filename = getName+".c"
    $stdout = File.open(filename, "w")

    print "// " + filename + "\n" + getMessage + 
      "#include        <stdio.h>\n" +
      "#include        <string.h>\n" +
      "#include        \"Giop.h\"\n" +
      "#include        \"Orbe.h\"\n" 
    generateIncludeHeaders
    print "\n\n"+
      "void dispatch(Req *req, Reply *reply, Giop_Hdr *header)\n" +
      "{\n" +
      "    u8      *req_ptr;\n" +
      "    u8      *reply_ptr;\n" +
      "    u8      *start;\n" +
      "    CORBA_ulong __length;\n" +
      "    req_ptr = req->body;\n" +
      "    reply_ptr = start = reply->body;\n\n"

    @children.each do |child| child.generateDispatch end

    print "  reply->body_size = (uint) (reply_ptr - start);\n" +
      "}\n"
    $stdout = STDOUT
  end
end

class IdlStruct < IdlElement
  def printHeaderInclude
    print "#include        \"" + getName + ".h\"\n"
  end

  def generateHeader
    filename = getName+".h"
    $stdout = File.open(filename, "w")
    filenameSymbol = "_" + getName.upcase + "_H"

    print  "// " + filename + "\n" + getMessage +
      "#ifndef " + filenameSymbol + "\n" +
      "#define " + filenameSymbol + "\n\n" +
      "#include        \"CORBA_Object.h\"\n"

    print "typedef struct " + getName + " {\n"

    @children.each do |child| child.generateHeader end

    print "} " + getName + ";\n\n" +
      "extern " + getName + " " + getName + "_Marshaller(void* ptr);\n\n" +
      "extern void " + getName + "_UnMarshaller(void* ptr, " + getName + " arg);\n\n" +
      "#endif //" + filenameSymbol + "\n"

    $stdout = STDOUT
  end

  def generateMarshaller
    filename = getName+".c"
    $stdout = File.open(filename, "w")

    print  "// " + filename + "\n" + getMessage +
      "#include        <stdio.h>\n" +
      "#include        <string.h>\n" +
      "#include        \"Giop.h\"\n" +
      "#include        \"Orbe.h\"\n" 

    generateIncludeHeaders

    print "\n\n"+
      getName + " " + getName + "_Marshaller(void* ptr)\n" +
      "{\n" + 
      "\t" + getName + " result;\n" + "\treturn result;\n";

    print "}\n\n"

    print "\n\n"+
      "void " + getName + "_UnMarshaller(void* ptr, " + getName + " arg)\n" +
      "{\n"

    print "}\n\n"

    $stdout = STDOUT
  end
end

class IdlException < IdlStruct
end

class IdlTypedElement < IdlElement
  def setTypeSpec(typeSpec)
    @typeSpec = typeSpec
  end

  def getTypeSpec
    return @typeSpec
  end

  def getTypeName
    return IdlMapping.getNativeTypeForIdlType(@typeSpec)
  end

  def getSize
    return IdlMapping.getSizeForIdlType(@typeSpec)
  end

  def getMarshaller
    return IdlMapping.getMarshallerForIdlType(@typeSpec)
  end

end

class IdlMember < IdlTypedElement
  def generateHeader
    print "\t" + getTypeName + "\t" + getName + ";\n"
  end
end


class IdlOperation < IdlTypedElement
  def setRaises(exception)
    @raises = exception
  end

  def setAttribute(attribute)
    @attribute = attribute
  end

  def getOpName
    return @parent.getName+"_"+@name
  end

  def getOpDeclaration
    result = getTypeName+" " + getOpName+"(" +
      "\n\t" + @parent.getName + " o,"
    @children.each do |child|
      result += "\n\t" + child.getTypeName 
      result += "*" if child.isOutParam
      result += " " + child.getName + ","
    end
    result += "\n\tCORBA_Environment* ev)"
    return result
  end

  def generateHeader
    print "extern " + getOpDeclaration + ";\n"
  end

  def generateTemplate
    print getOpDeclaration + "\n{\n\t// TODO: implement here\n" +
      "}\n\n"
  end

  def generateDispatch
    print "    if (strcmp((void*) req->operation, \"" + getName + "\") == 0) {\n"

    print "\t// incoming values\n"    
    @children.each do |child|
      if child.getTypeName=='CORBA_Object'
        print "\tstruct corba_object " + child.getName + "_value;\n" +
          "\tCORBA_Object " + child.getName + " = &" + 
          child.getName + "_value"
      else
        print "\t" + child.getTypeName + " " + child.getName
      end

      if not child.isInParam
        print ";\n\n"
        next 
      end

      marshaller = child.getMarshaller;
      if marshaller=='pObjectPtrMarshaller'
        print ";\n" + 
          "\t(*header->marshall." + marshaller + ")"
        print "(" + child.getName + ", req_ptr, header);\n" +
          "\treq_ptr += " + child.getName + "->length;\n" +
          "\treq_ptr += ALIGN(4, " + child.getName + "->length);\n" +
          "\n"
      else
        if marshaller == nil
          print " = " + child.getTypeName + "_Marshaller"
        elsif marshaller=='*'
          print " = *"
        else
          print " = (*header->marshall." + marshaller + ")"
        end

        print "((" +
          child.getTypeName + "*)req_ptr);\n" +
          "\treq_ptr += " + child.getSize.to_s + ";\n\n"
      end
    end
      
    print "\t" 
    if @typeSpec=='Object'
      print "struct corba_object result_value;\n" +
        "\tCORBA_Object result = &result_value;\n" +
        "\tresult_value = *"
    else
      print getTypeName + " result = " if getTypeName!='void'
    end

    print getOpName + "(NULL, "
    
    @children.each do |child|
      print "&" if child.isOutParam
      print child.getName
      print ", "
    end

    print "NULL);\n\n"

    print "\t// return value\n"
    if @typeSpec == 'Object'
      print "\t{\n" +
        "\tmemcpy(reply_ptr, result->object_key, result->length);\n" +
        "\treply_ptr += result->length;\n" +
        "\tint tmp = ALIGN(4, result->length);\n" +
        "\twhile (tmp-- > 0) {*reply_ptr++ = '\\0';}\n\t}\n\n"
    elsif @typeSpec == 'string'
      print "\t__length = strlen(result) + 1;\n" +
        "\t*((CORBA_ulong*)reply_ptr) = __length;\n" +
        "\treply_ptr += 4;\n" +
        "\tmemcpy(reply_ptr, result, __length);\n" +
        "\treply_ptr += __length;\n\n"
    else
      print "\tmemcpy(reply_ptr, (u8*) &result, " + getSize.to_s + ");\n" +
        "\treply_ptr += " + getSize.to_s + ";\n\n" if getTypeName!='void'
    end

    print "\t// outgoing values\n"
    @children.each do |child|
      next if not child.isOutParam
      if child.getTypeSpec == 'Object'
        print "\t{\n" +
          "\tmemcpy(reply_ptr, "+child.getName+"->object_key, "+
          child.getName+"->length);\n" +
          "\treply_ptr += "+child.getName+"->length;\n" +
          "\tint tmp = ALIGN(4, "+child.getName+"->length);\n" +
          "\twhile (tmp-- > 0) {*reply_ptr++ = '\\0';}\n\t}\n\n"
      elsif child.getTypeSpec == 'string'
        print "\t__length = strlen(" + child.getName + ") + 1;\n" +
          "\t*((CORBA_ulong*)reply_ptr) = __length;\n" +
          "\treply_ptr += 4;\n" +
          "\tmemcpy(reply_ptr, " + child.getName + ", __length);\n" +
          "\treply_ptr += __length;\n\n"
      else
        print "\tmemcpy(reply_ptr, (u8*) &" + child.getName + ", " + 
          child.getSize.to_s + ");\n" +
          "\treply_ptr += " + child.getSize.to_s + ";\n\n"
      end
    end

    print "    }\n"

  end
end

class IdlParameter < IdlTypedElement
  def setAttribute(attribute)
    @attribute = attribute
  end

  def isInParam
    return true if @attribute == "in"
    return true if @attribute == "inout"
    return false
  end
  
  def isOutParam
    return true if @attribute == "out"
    return true if @attribute == "inout"
    return false
  end

end

class IdlTypeSpec < IdlElement
end

class IdlMapping
  @@typeTable = { 
                 'void'=>'void',
                 'short'=>'CORBA_short',
                 'unsigned short'=>'CORBA_ushort',
                 'long'=>'CORBA_long',
                 'unsigned long'=>'CORBA_long',
                 'float'=>'CORBA_float',
                 'double'=>'CORBA_double',
                 'char'=>'CORBA_char',
                 'boolean'=>'CORBA_boolean',
                 'octet'=>'CORBA_octet',
                 'any'=>'CORBA_any',
                 'string'=>'CORBA_string',
                 'Object'=>'CORBA_Object',
                 'TypeCode'=>'CORBA_TypeCode'
               }

  @@sizeTable = { 
                 'void'=>0,
                 'short'=>2,
                 'unsigned short'=>2,
                 'long'=>4,
                 'unsigned long'=>4,
                 'float'=>4,
                 'double'=>8,
                 'char'=>1,
                 'boolean'=>1,
                 'octet'=>1,
                 'any'=>8,
                 'string'=>4,
                 'Object'=>4,
                 'TypeCode'=>4
               }

  @@marshallerTable = { 
    'void'=>'*',
    'short'=>'pShortmarshaller',
    'unsigned short'=>'pShortmarshaller',
    'long'=>'pIntmarshaller',
    'unsigned long'=>'pIntmarshaller',
    'float'=>'pFloatmarshaller',
    'double'=>'pDoublemarshaller',
    'char'=>'*',
    'boolean'=>'*',
    'octet'=>'*',
    'any'=>'pAnyMarshaller',
    'string'=>'pStringMarshaller',
    'Object'=>'pObjectPtrMarshaller',
    'TypeCode'=>'pIntmarshaller'
  }



  def self.getNativeTypeForIdlType(idlType)
    if @@typeTable.key?(idlType)
      return @@typeTable.fetch(idlType)
    end

    return idlType
  end

  def self.getSizeForIdlType(idlType)
    if @@sizeTable.key?(idlType)
      return @@sizeTable.fetch(idlType)
    end

    return 0
  end

  def self.getMarshallerForIdlType(idlType)
    if @@marshallerTable.key?(idlType)
      return @@marshallerTable.fetch(idlType)
    end

    return nil
  end

end


#test code
#
#spec = IdlSpecification.new("root")
#
#rm11 = IdlInterface.new("rm11")
#rm1  = IdlInterface.new("rm1")
#
#spec.addChild(rm11)
#spec.addChild(rm1)
#
#spec.generate

