# Test: mips3

		.text	
		addiu	$t0, $0, 2
		addiu	$t1, $0, 2
		subu	$t2, $t1, $t0
		andi	$t3, $t0, 1
		and	$t4, $t2, $t1
		bne	$t3, $t4, Done
		
Done:
		jr	$ra