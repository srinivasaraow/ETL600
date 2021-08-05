#rem -------------------------------------------------------------------------------
#rem -------------------------------------------------------------------------------
#rem -- ABB Switzerland Ltd. All rights reserved.
#rem -------------------------------------------------------------------------------
#rem -- Author(en):   Mathias Gloor
#rem -- Date:         05.07.2004
#rem --
#rem -- Updates: Date, Version, Remarks, Author       
#rem -- 10.10.2002, , dat2dcc adaptation, Mathias Gloor
#rem -- 15.01.2003, , include synthesis, Mathias Gloor
#rem -- 05.07.2004, , adaptation for NSK600
#rem -- 19.03.2008, , adaptation for PC Stefan
#rem -- 05.10.2010, , adaptation for PC 2 Stefan
#rem -------------------------------------------------------------------------------
#rem -- Description:
#rem -- Implementation script for the NSK600 design
#rem -- 
#rem -------------------------------------------------------------------------------
#rem -------------------------------------------------------------------------------

compile
synthesize 
place_and_route cl 


set PROJECT_DIR C:/Projects/local_FPGA_databases/ETL600/FPGA/NSK_FPGA/NSK600_lib
set TEMP_FOLDER nsk600_top_struct/nsk600_top_struct_impl_1

cd $PROJECT_DIR/implementation

puts "\nexecute bitgen"
exec bitgen -g DonePin:Pullnone -w $TEMP_FOLDER/nsk600_top_out.ncd  -l -b

puts "\ncopy bit-file"
file copy -force $TEMP_FOLDER/nsk600_top_out.bit nsk600.bit

puts "\ncopy edif-file / just for Visual Source Safe handling"
file copy -force $TEMP_FOLDER/nsk600_top.edf nsk600.edf

puts "\nexecute promgen"
exec promgen -p hex -s 2048 -u 0 nsk600 -w
puts "\nHEX-file generated"
#str puts "\ncopy hex-file to the local ADSP path"
#str file copy -force ./nsk600.hex D:/ADSP/\$P4LT/NSK_DSP/batch/nsk600.hex


#exec ss cp $/Source/FPGA/NSK_FPGA/implementation  # SourceSafe current project
#exec ss Checkin nsk600.hex -K                     # SourceSafe Check In the image
#puts "\nHEX-file checked in !"
puts "\nScript successfully finished"
