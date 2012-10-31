##
##
## $Id: sample_packet.rb 849 2008-10-16 08:27:22Z takaya $
##

## LocateReq (omniORB)
buf =   "47 49 4f 50 01 02 01 03 1a 00 00 00 02 00 00 00"
buf +=  "00 00 00 00 0e 00 00 00 fe e4 f6 45 48 00 00 4e"
buf +=  "1c 00 00 00 00 00"

## Req (omniORB)

buf  =  "47 49 4f 50 01 02 01 00 48 00 00 00 04 00 00 00"        # 16
buf +=  "03 00 00 00 00 00 00 00 0e 00 00 00 fe e4 f6 45"        # 32
buf +=  "48 00 00 4e 1c 00 00 00 00 00 5b 09 04 00 00 00"        # 48
buf +=  "6f 70 32 00 01 00 00 00 01 00 00 00 0c 00 00 00"        # 64
buf +=  "01 00 00 00 01 00 01 00 09 01 01 00 00 00 00 00"        # 80
buf +=  "9c ff 9c ff"

## Req (javaIDL)

buf =   "47 49 4f 50 01 02"			# GIOP..
buf +=  "00 00 00 00 01 38 00 00 00 05 03 00 00 00 00 00"#  .....8.. ........
buf +=  "00 00 00 00 00 31 af ab cb 00 00 00 00 20 b4 55"#   .....1.. ..... .U
buf +=  "67 98 00 00 00 01 00 00 00 00 00 00 00 01 00 00"#   g....... ........
buf +=  "00 08 52 6f 6f 74 50 4f 41 00 00 00 00 08 00 00"#   ..RootPO A.......
buf +=  "00 01 00 00 00 00 14 00 00 00 00 00 00 04 6f 70"#   ........ ......op
buf +=  "31 00 00 00 00 04 00 00 00 11 00 00 00 02 00 02"#   1....... ........
buf +=  "00 00 00 00 00 01 00 00 00 0c 00 00 00 00 00 01"#   ........ ........
buf +=  "00 01 00 01 01 09 4e 45 4f 00 00 00 00 02 00 14"#   ......NE O.......
buf +=  "00 00 00 00 00 06 00 00 00 aa 00 00 00 00 00 00"#   ........ ........
buf +=  "00 28 49 44 4c 3a 6f 6d 67 2e 6f 72 67 2f 53 65"#   .(IDL:om g.org/Se
buf +=  "6e 64 69 6e 67 43 6f 6e 74 65 78 74 2f 43 6f 64"#   ndingCon text/Cod
buf +=  "65 42 61 73 65 3a 31 2e 30 00 00 00 00 01 00 00"#   eBase:1. 0.......
buf +=  "00 00 00 00 00 6e 00 01 02 00 00 00 00 0d 31 39"#   .....n.. ......\x19
buf +=  "32 2e 31 36 38 2e 31 2e 31 33 00 00 12 01 00 00"#   2.\x168.1. \x13......
buf +=  "00 19 af ab cb 00 00 00 00 02 c7 d4 3b 13 00 00"#   ........ ....;...
buf +=  "00 08 00 00 00 00 00 00 00 00 14 00 00 00 00 00"#   ........ ........
buf +=  "00 02 00 00 00 01 00 00 00 20 00 00 00 00 00 01"#   ........ . ......
buf +=  "00 01 00 00 00 02 05 01 00 01 00 01 00 20 00 01"#   ........ ..... ..
buf +=  "01 09 00 00 00 01 00 01 01 00 00 00 00 26 00 00"#   ........ .....&..
buf +=  "00 02 00 02 00 00 00 00 00 00 ff 9c ff 9c"#.


##  Req (javaIDL 2)

buf =  "47 49 4f 50 01 02"                                 # .......L ..GIOP..
buf += "00 00 00 00 01 38 00 00 00 05 03 00 00 00 00 00"   # .....8.. ........
buf += "00 00 00 00 00 31 af ab cb 00 00 00 00 20 b4 55"   # .....1.. ..... .U
buf += "67 98 00 00 00 01 00 00 00 00 00 00 00 01 00 00"   # g....... ........
buf += "00 08 52 6f 6f 74 50 4f 41 00 00 00 00 08 00 00"   # ..RootPO A.......
buf += "00 01 00 00 00 00 14 00 00 00 00 00 00 04 6f 70"   # ........ ......op
buf += "31 00 00 00 00 04 00 00 00 11 00 00 00 02 00 02"   # 1....... ........
buf += "00 00 00 00 00 01 00 00 00 0c 00 00 00 00 00 01"   # ........ ........
buf += "00 01 00 01 01 09 4e 45 4f 00 00 00 00 02 00 14"   # ......NE O.......
buf += "00 00 00 00 00 06 00 00 00 aa 00 00 00 00 00 00"   # ........ ........
buf += "00 28 49 44 4c 3a 6f 6d 67 2e 6f 72 67 2f 53 65"   # .(IDL:om g.org/Se
buf += "6e 64 69 6e 67 43 6f 6e 74 65 78 74 2f 43 6f 64"   # ndingCon text/Cod
buf += "65 42 61 73 65 3a 31 2e 30 00 00 00 00 01 00 00"   # eBase:1. 0.......
buf += "00 00 00 00 00 6e 00 01 02 00 00 00 00 0d 31 39"   # .....n.. ......19
buf += "32 2e 31 36 38 2e 31 2e 31 33 00 00 09 87 00 00"   # 2.168.1. 13......
buf += "00 19 af ab cb 00 00 00 00 02 dc 4e 66 30 00 00"   # ........ ...Nf0..
buf += "00 08 00 00 00 00 00 00 00 00 14 00 00 00 00 00"   # ........ ........
buf += "00 02 00 00 00 01 00 00 00 20 00 00 00 00 00 01"   # ........ . ......
buf += "00 01 00 00 00 02 05 01 00 01 00 01 00 20 00 01"   # ........ ..... ..
buf += "01 09 00 00 00 01 00 01 01 00 00 00 00 26 00 00"   # ........ .....&..
buf += "00 02 00 02 00 00 00 00 00 00 ff 9c ff 9c"           # ........ ......  

## op1 (rm11op1 javaIDL)

buf =  "00 01 02 03 04 05 00 07  e9 1a 44 4d 08 00 45 00 "	#   ........ ..DM..E.
buf += "01 60 93 f0 00 00 80 11  22 35 c0 a8 01 0d c0 a8 "	#   .`...... "5......
buf += "01 0a 05 54 16 18 01 4c  96 72 47 49 4f 50 01 02 "	#   ...T...L .rGIOP..
buf += "00 00 00 00 01 38 00 00  00 05 03 00 00 00 00 00 "	#   .....8.. ........
buf += "00 00 00 00 00 31 af ab  cb 00 00 00 00 20 e1 71 "	#   .....1.. ..... .q
buf += "c9 db 00 00 00 01 00 00  00 00 00 00 00 01 00 00 "	#   ........ ........
buf += "00 08 52 6f 6f 74 50 4f  41 00 00 00 00 08 00 00 "	#   ..RootPO A.......
buf += "00 01 00 00 00 00 14 00  00 00 00 00 00 04 6f 70 "	#   ........ ......op
buf += "31 00 00 00 00 04 00 00  00 11 00 00 00 02 00 02 "	#   1....... ........
buf += "00 00 00 00 00 01 00 00  00 0c 00 00 00 00 00 01 "	#   ........ ........
buf += "00 01 00 01 01 09 4e 45  4f 00 00 00 00 02 00 14 "	#   ......NE O.......
buf += "00 00 00 00 00 06 00 00  00 aa 00 00 00 00 00 00 "	#   ........ ........
buf += "00 28 49 44 4c 3a 6f 6d  67 2e 6f 72 67 2f 53 65 "	#   .(IDL:om g.org/Se
buf += "6e 64 69 6e 67 43 6f 6e  74 65 78 74 2f 43 6f 64 "	#   ndingCon text/Cod
buf += "65 42 61 73 65 3a 31 2e  30 00 00 00 00 01 00 00 "	#   eBase:1. 0.......
buf += "00 00 00 00 00 6e 00 01  02 00 00 00 00 0d 31 39 "	#   .....n.. ......19
buf += "32 2e 31 36 38 2e 31 2e  31 33 00 00 05 57 00 00 "	#   2.168.1. 13...W..
buf += "00 19 af ab cb 00 00 00  00 02 fb 48 39 b1 00 00 "	#   ........ ...H9...
buf += "00 08 00 00 00 00 00 00  00 00 14 00 00 00 00 00 "	#   ........ ........
buf += "00 02 00 00 00 01 00 00  00 20 00 00 00 00 00 01 "	#   ........ . ......
buf += "00 01 00 00 00 02 05 01  00 01 00 01 00 20 00 01 "	#   ........ ..... ..
buf += "01 09 00 00 00 01 00 01  01 00 00 00 00 26 00 00 "	#   ........ .....&..
buf += "00 02 00 02 00 00 00 00  00 00 ff 9c ff 9c "

## Array Operation

buf =  "00 01 02 03 04 05 00 07  e9 1a 44 4d 08 00 45 00 "	#   ........ ..DM..E.
buf += "01 60 93 f0 00 00 80 11  22 35 c0 a8 01 0d c0 a8 "	#   .`...... "5......
buf += "01 0a 05 54 16 18 01 4c  96 72 47 49 4f 50 01 02 "	#   ...T...L .rGIOP..
buf += "00 00 00 00 02 c4 00 00  00 05 03 00 00 00 00 00 "	#   .....8.. ........
buf += "00 00 00 00 00 31 af ab  cb 00 00 00 00 20 e1 71 "	#   .....1.. ..... .q
buf += "c9 db 00 00 00 01 00 00  00 00 00 00 00 01 00 00 "	#   ........ ........
buf += "00 08 52 6f 6f 74 50 4f  41 00 00 00 00 08 00 00 "	#   ..RootPO A.......
buf += "00 01 00 00 00 00 14 00  00 00 00 00 00 04 6f 70 "	#   ........ ......op
buf += "30 00 00 00 00 04 00 00  00 11 00 00 00 02 00 02 "	#   0....... ........
buf += "00 00 00 00 00 01 00 00  00 0c 00 00 00 00 00 01 "	#   ........ ........
buf += "00 01 00 01 01 09 4e 45  4f 00 00 00 00 02 00 14 "	#   ......NE O.......
buf += "00 00 00 00 00 06 00 00  00 aa 00 00 00 00 00 00 "	#   ........ ........
buf += "00 28 49 44 4c 3a 6f 6d  67 2e 6f 72 67 2f 53 65 "	#   .(IDL:om g.org/Se
buf += "6e 64 69 6e 67 43 6f 6e  74 65 78 74 2f 43 6f 64 "	#   ndingCon text/Cod
buf += "65 42 61 73 65 3a 31 2e  30 00 00 00 00 01 00 00 "	#   eBase:1. 0.......
buf += "00 00 00 00 00 6e 00 01  02 00 00 00 00 0d 31 39 "	#   .....n.. ......19
buf += "32 2e 31 36 38 2e 31 2e  31 33 00 00 05 57 00 00 "	#   2.168.1. 13...W..
buf += "00 19 af ab cb 00 00 00  00 02 fb 48 39 b1 00 00 "	#   ........ ...H9...
buf += "00 08 00 00 00 00 00 00  00 00 14 00 00 00 00 00 "	#   ........ ........
buf += "00 02 00 00 00 01 00 00  00 20 00 00 00 00 00 01 "	#   ........ . ......
buf += "00 01 00 00 00 02 05 01  00 01 00 01 00 20 00 01 "	#   ........ ..... ..
buf += "01 09 00 00 00 01 00 01  01 00 00 00 00 26 00 00 "	#   ........ .....&..
buf += "00 02 00 02 00 00 00 00  00 00 "
buf += "ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c "
buf += "ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c "
buf += "ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c "
buf += "ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c "
buf += "ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c ff ff ff 9c "

## op12 (rm11op12 javaIDL)

buf =  "00 00 01 02 03 06 00 1E 37 22 56 18 08 00 45 00 "
buf += "01 F2 E0 F7 00 00 80 11 D4 6E C0 A8 01 0B C0 A8 "
buf += "01 39 05 28 16 18 01 DE 82 56 47 49 4F 50 01 02 "
buf += "00 00 00 00 01 CA 00 00 00 06 03 00 00 00 00 00 "
buf += "00 00 00 00 00 31 AF AB CB 00 00 00 00 20 AF 94 "
buf += "78 D6 00 00 00 01 00 00 00 00 00 00 00 01 00 00 "
buf += "00 08 52 6F 6F 74 50 4F 41 00 00 00 00 08 00 00 "
buf += "00 01 00 00 00 00 14 00 00 00 00 00 00 05 6F 70 "
buf += "31 32 00 00 00 04 00 00 00 03 00 00 00 11 00 00 "
buf += "00 02 00 02 00 01 00 00 00 01 00 00 00 0C 00 00 "
buf += "00 00 00 01 00 01 00 01 01 09 4E 45 4F 00 00 00 "
buf += "00 02 00 14 00 06 00 00 00 AA 00 00 00 0D 49 44 "
buf += "4C 3A 72 6D 31 31 3A 31 2E 30 00 72 67 2F 00 00 "
buf += "00 01 00 00 00 00 00 00 00 82 00 01 02 00 00 00 "
buf += "00 0A 31 32 37 2E 30 2E 30 2E 31 00 16 18 00 00 "
buf += "00 31 AF AB CB 00 00 00 00 20 AF 94 78 D6 00 00 "
buf += "00 01 00 00 00 00 00 00 00 01 00 00 00 08 52 6F "
buf += "6F 74 50 4F 41 00 00 00 00 08 00 00 00 01 00 00 "
buf += "00 00 14 00 00 00 00 00 00 02 00 00 00 01 00 00 "
buf += "00 20 00 00 00 00 00 01 00 01 00 00 00 02 05 01 "
buf += "00 01 00 01 00 20 00 01 01 09 00 00 00 01 00 01 "
buf += "01 00 00 00 00 26 00 00 00 02 00 02 00 26 00 00 "
buf += "00 0D 49 44 4C 3A 72 6D 31 31 3A 31 2E 30 00 00 "
buf += "00 00 00 00 00 01 00 00 00 00 00 00 00 82 00 01 "
buf += "02 00 00 00 00 0A 31 32 37 2E 30 2E 30 2E 31 00 "
buf += "16 18 00 00 00 31 AF AB CB 00 00 00 00 20 AF 94 "
buf += "78 D6 00 00 00 01 00 00 00 00 00 00 00 01 00 00 "
buf += "00 08 52 6F 6F 74 50 4F 41 00 00 00 00 08 00 00 "
buf += "00 01 00 00 00 00 14 00 00 00 00 00 00 02 00 00 "
buf += "00 01 00 00 00 20 00 00 00 00 00 01 00 01 00 00 "
buf += "00 02 05 01 00 01 00 01 00 20 00 01 01 09 00 00 "
buf += "00 01 00 01 01 00 00 00 00 26 00 00 00 02 00 02 "

buf.gsub!(/.* 47 49 4f 50/i, "47 49 4f 50")
tmp = []
buf.split.each{|x|
	tmp << x.hex
}

print tmp.pack("c*")
