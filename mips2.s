# Test: mips2



		.text	
		addiu	$t0, $0, 1
		addiu	$t1, $0, 2
		addiu	$t2, $0, 3
		slt	$t3, $t1, $t0
		or	$t4, $t1, $t2
		ori	$t5, $t1, 1