#! /usr/bin/tclsh

package require Tk

proc reader {serial} {
	if {![binary scan [read $serial 4] cccc x y z t]} {
		error "reading data!"
	}
#	puts "x = $x"
#	puts "y = $y"
#	puts "z = $z"
#	puts "t = $t"
	set ::xout "xout: $x"
	set ::yout "yout: $y"
	set ::zout "zout: $z"
	set ::tilt [format "tilt: 0x%x" $t]
	
	set s ""
	switch [expr $t & 3 ] {
		1 {
			set s "$s, front"
		}
		2 {
			set s "$s, back"
		}
	}
	switch [expr ($t >> 2) & 7] {
		1 {
			set s "$s, left"
		}
		2 {
			set s "$s, right"
		}
		5 {
			set s "$s, down"
		}
		6 {
			set s "$s, up"
		}
	}
	if {$t & 0x20} {
			set s "$s, tapped"
	}
	if {$t & 0x80} {
			set s "$s, shaken"
	}
	set ::tilt "$::tilt $s"
}

set xout "xout: "
set yout "yout: "
set zout "zout: "
set tilt "tilt: "

set serial [open /dev/ttyACM0 r+]
fconfigure $serial -mode 115200,n,8,1 -buffering none -blocking 1 -encoding ascii -translation binary
fileevent $serial readable [list reader $serial]

wm title . "accelerometer"

label .xout -textvariable ::xout -anchor w
label .yout -textvariable ::yout -anchor w
label .zout -textvariable ::zout -anchor w
label .tilt -textvariable ::tilt -anchor w
button .b -text "update" -width 20 -command {puts -nonewline $serial "m"}

pack .xout .yout .zout .tilt .b -expand true -fill x
