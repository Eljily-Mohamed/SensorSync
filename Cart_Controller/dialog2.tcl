#! /usr/bin/tclsh

package require Tk

proc reader {serial} {
	if {![binary scan [read $serial 4] cccc x y z t]} {
		error "reading data!"
	}
	set ff "%+5.3fg"
	set ::xout "xout: [format $ff [expr $x]]"
	set ::yout "yout: [format $ff [expr $y]]"
	set ::zout "zout: [format $ff [expr $z]]"
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

proc get_mm7660_xyzt {} {
	puts -nonewline $::serial "m"
	set ::pending [after $::dt get_mm7660_xyzt]
}

# update period
set dt 100

set xout "xout: "
set yout "yout: "
set zout "zout: "
set tilt "tilt: "

set serial [open /dev/ttyACM0 r+]
fconfigure $serial -mode 115200,n,8,1 -buffering none -blocking 1 -encoding ascii -translation binary
fileevent $serial readable [list reader $serial]

wm title . "accelerometer"

label .xout -textvariable ::xout -anchor w -font TkFixedFont
label .yout -textvariable ::yout -anchor w -font TkFixedFont
label .zout -textvariable ::zout -anchor w -font TkFixedFont
label .tilt -textvariable ::tilt -anchor w
button .start -text "start" -width 20 -command {
	set ::pending [after $::dt get_mm7660_xyzt]
	.stop configure -state normal
	.start configure -state disabled
}
button .stop -text "stop" -state disabled -command {
	after cancel $::pending
	.start configure -state normal
	.stop configure -state disabled
}

pack .xout .yout .zout .tilt .start .stop -expand true -fill x
