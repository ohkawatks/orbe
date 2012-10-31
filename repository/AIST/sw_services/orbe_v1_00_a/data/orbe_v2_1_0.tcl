proc orbe_drc {lib_handle} {
    set sw_processor [xget_libgen_proc_handle]
    set processor [xget_handle $sw_processor "IPINST"]

    set periphs_list [xget_proc_slave_periphs $processor]
    set found 0

    foreach periph $periphs_list {
	set periphname [xget_value $periph]
	if {$periphname == "plb_orbe" || $periphname == "fsl_orbe"} {
	    set found 1
	}
    }
    if {$found == 0} {
#	error "ORB Engine Hw module is not found.\n" "" "MDT_ERROR"
	return
    }
}

proc generate {lib_handle} {
	set conffile [ open "src/Makefile.config" w ]
	set confhdr  [ xopen_include_file "xparameters.h" ]

	set socket_udp    [ xget_value $lib_handle "PARAMETER" "CONFIG_SOCKET_CHANNEL_USE_UDP" ]
	set socket_uart   [ xget_value $lib_handle "PARAMETER" "CONFIG_SOCKET_CHANNEL_USE_UART" ]
	set orbe_timer   [ xget_value $lib_handle "PARAMETER" "CONFIG_ORBE_TIMER" ]

	if {$socket_udp} {
		puts $conffile "CONFIG_SOCKET_CHANNEL_USE_UDP = y"
	}
	if {$socket_uart} {
		puts $conffile "CONFIG_SOCKET_CHANNEL_USE_UART = y"
	}
	if {$orbe_timer} {
		puts $conffile "CONFIG_ORBE_TIMER = y"
	}

	set ip_address  [xget_value $lib_handle "PARAMETER" "CONFIG_ORBE_IP_ADDRESS"]
	puts "IP Address = $ip_address"
	regsub -all {\.} $ip_address {,} ip_address
	puts $confhdr 	"#define CONFIG_ORBE_IP_ADDRESS $ip_address"

	set mac_address  [xget_value $lib_handle "PARAMETER" "CONFIG_ORBE_MAC_ADDRESS"]
	puts "MAC Address = $mac_address"
	regsub -all {\-} $mac_address {,} mac_address
	puts $confhdr 	"#define CONFIG_ORBE_MAC_ADDRESS $mac_address"

	close $conffile
	close $confhdr	
}
