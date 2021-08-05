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

####################################################################################
compile

####################################################################################
#set_attribute -instance i6_config.i6_4_lut_brt.* -name block_rom -value false
synthesize 


set PROJECT_DIR C:/Project_ETL600/IC_Design/NSK600_lib
set TEMP_FOLDER nsk600_top_struct/nsk600_top_struct_temp_1

cd $PROJECT_DIR/implementation

puts "\ncopy edif-file / just for Visual Source Safe handling"
file copy -force $TEMP_FOLDER/nsk600_top.edf nsk600.edf



####################################################################################
place_and_route cl 

#set UCF_FILE $PROJECT_DIR/configuration/nsk600.ucf
#edif2ngd nsk600.edf 
#ngdbuild -aul nsk600 -uc $UCF_FILE
#map -pr b -bp nsk600.ngd -o nsk600.ncd nsk600.pcf
#par -w -ol 5 -xe 1 -t 1 -n 1 -ub nsk600.ncd nsk600_routed nsk600.pcf
#trce -v 100 nsk600_routed.ncd nsk600.pcf
##rem ngdanno -o %DESIGN_TOP_ROUTED%.nga -p %DESIGN_TOP%.pcf %DESIGN_TOP_ROUTED%.ncd %DESIGN_TOP%.ngm
##rem ngd2vhdl -w %DESIGN_TOP_ROUTED%.nga

puts "\nexecute bitgen"
exec bitgen -w $TEMP_FOLDER/nsk600_top_out.ncd  -l -b

puts "\ncopy bit-file"
file copy -force $TEMP_FOLDER/nsk600_top_out.bit nsk600.bit

puts "\nexecute promgen (please verify writing permission for the hex file)"
exec promgen -p hex -s 2048 -u 0 nsk600 -w
puts "\nHEX-file generated"
#puts "\ncopy hex-file to the local ADSP path"
#file copy -force ./nsk600.hex D:/ADSP/$P4LT/NSK_DSP/batch/nsk600.hex

#puts "\ncopy hex-file to the personal k-drive"
#file copy -force ./nsk600.hex K:/nsk600.hex


#exec ss cp $/Source/FPGA/NSK_FPGA/implementation  # SourceSafe current project
#exec ss Checkin nsk600.hex -K                     # SourceSafe Check In the image
#puts "\nHEX-file checked in !"
puts "\nScript successfully finished"
