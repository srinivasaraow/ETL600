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
#rem -------------------------------------------------------------------------------
#rem -- Description:
#rem -- Implementation script for the NSK600 design
#rem -- 
#rem -------------------------------------------------------------------------------
#rem -------------------------------------------------------------------------------

compile
synthesize 
place_and_route cl 


set PROJECT_DIR D:/ETL600/FPGA/hdl_projects/NSK600_lib
set TEMP_FOLDER nsk600_top_struct/nsk600_top_struct_temp_1

cd $PROJECT_DIR/implementation

puts "\nexecute bitgen"
exec bitgen -w $TEMP_FOLDER/nsk600_top_out.ncd  -l -b

puts "\ncopy bit-file"
file copy -force $TEMP_FOLDER/nsk600_top_out.bit nsk600.bit

puts "\ncopy edif-file / just for Visual Source Safe handling"
file copy -force $TEMP_FOLDER/nsk600_top.edf nsk600.edf

puts "\nexecute promgen"
exec promgen -p hex -s 2048 -u 0 nsk600 -w
puts "\nHEX-file generated"
puts "\ncopy hex-file to the local ADSP path"
file copy -force ./nsk600.hex D:/ADSP/\$P4LT/NSK_DSP/batch/nsk600.hex


#exec ss cp $/Source/FPGA/NSK_FPGA/implementation  # SourceSafe current project
#exec ss Checkin nsk600.hex -K                     # SourceSafe Check In the image
#puts "\nHEX-file checked in !"
puts "\nScript successfully finished"
