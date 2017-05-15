# 
# (c) Copyrights 2008 HITLabNZ, Hartmut Seichter
# 
 
find_package(ARToolKit2)
find_package(SSTT)

if(ARTOOLKIT2_FOUND)
	add_subdirectory(ARToolKit)
endif(ARTOOLKIT2_FOUND)

if(SSTT_FOUND)
	add_subdirectory(SSTT)
endif(SSTT_FOUND) 

