# Test: mips1

		.text	
		addiu	$t0, $0, 1
		addu	$t1, $t0, $t0
		lui	$t0, 0x0040
		addu	$t2, $t0, $t1
		sw	$t1, 0($t0)
		lw	$t2, 0($t0)
		sll	$t2, $t2, 4
		srl	$t2, $t2, 4
		addiu	$0, $0, 0
