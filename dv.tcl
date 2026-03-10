    set ns [new Simulator]
$ns rtproto DV
set nf [open dv1.tr w]
$ns trace-all $nf
set nr [open dv2.nam w]
$ns namtrace-all $nr
proc finish {} {
    global ns nf nr
    $ns flush-trace
    close $nf
    close $nr
    #exec nam dv2.nam
    exit 0
}
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n0 1Mb 10ms DropTail
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
set null0 [new Agent/Null]
$ns attach-agent $n3 $null0
$ns connect $udp0 $null0
$ns at .1 "$cbr0 start"
$ns at 45.0 "$cbr0 stop"
$ns at 50.0 "finish"
$ns run
