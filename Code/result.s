.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0 5
syscall
jr $ra

write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra


myadd:
sw $a0, 4($sp)
sw $a1, 8($sp)
sw $a2, 12($sp)
sw $a3, 16($sp)
lw $t1, 0($sp)
sw $t1, 20($sp)
lw $t1, 4($sp)
sw $t1, 4($sp)
lw $t2, 4($sp)
move $t1, $t2
sw $t1, 24($sp)
lw $t1, 24($sp)
move $a0, $t1
addi $sp, $sp, 32
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -32
lw $t1, 24($sp)
move $t1,$v0
sw $t1, 24($sp)
lw $t2, 8($sp)
move $t1, $t2
sw $t1, 28($sp)
lw $t1, 28($sp)
move $a0, $t1
addi $sp, $sp, 36
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -36
lw $t1, 28($sp)
move $t1,$v0
sw $t1, 28($sp)
lw $t2, 12($sp)
move $t1, $t2
sw $t1, 32($sp)
lw $t1, 32($sp)
move $a0, $t1
addi $sp, $sp, 40
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -40
lw $t1, 32($sp)
move $t1,$v0
sw $t1, 32($sp)
lw $t2, 16($sp)
move $t1, $t2
sw $t1, 36($sp)
lw $t1, 36($sp)
move $a0, $t1
addi $sp, $sp, 44
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -44
lw $t1, 36($sp)
move $t1,$v0
sw $t1, 36($sp)
lw $t2, 20($sp)
move $t1, $t2
sw $t1, 40($sp)
lw $t1, 40($sp)
move $a0, $t1
addi $sp, $sp, 48
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -48
lw $t1, 40($sp)
move $t1,$v0
sw $t1, 40($sp)
lw $t2, 4($sp)
move $t1, $t2
sw $t1, 44($sp)
lw $t2, 8($sp)
move $t1, $t2
sw $t1, 48($sp)
lw $t3, 48($sp)
lw $t2, 44($sp)
add $t1, $t2, $t3
sw $t1, 52($sp)
lw $t2, 12($sp)
move $t1, $t2
sw $t1, 56($sp)
lw $t3, 56($sp)
lw $t2, 52($sp)
add $t1, $t2, $t3
sw $t1, 60($sp)
lw $t2, 16($sp)
move $t1, $t2
sw $t1, 64($sp)
lw $t3, 64($sp)
lw $t2, 60($sp)
add $t1, $t2, $t3
sw $t1, 68($sp)
lw $t2, 20($sp)
move $t1, $t2
sw $t1, 72($sp)
lw $t3, 72($sp)
lw $t2, 68($sp)
add $t1, $t2, $t3
sw $t1, 76($sp)
lw $t1, 76($sp)
move $v0, $t1
jr $ra

main:
addi $sp, $sp, 4
sw $ra, -4($sp)
jal read
lw $ra, -4($sp)
addi $sp, $sp, -4
move $t1,$v0
sw $t1, 0($sp)
lw $t2, 0($sp)
move $t1, $t2
sw $t1, 4($sp)
addi $sp, $sp, 12
sw $ra, -4($sp)
jal read
lw $ra, -4($sp)
addi $sp, $sp, -12
move $t1,$v0
sw $t1, 8($sp)
lw $t2, 8($sp)
move $t1, $t2
sw $t1, 12($sp)
addi $sp, $sp, 20
sw $ra, -4($sp)
jal read
lw $ra, -4($sp)
addi $sp, $sp, -20
move $t1,$v0
sw $t1, 16($sp)
lw $t2, 16($sp)
move $t1, $t2
sw $t1, 20($sp)
addi $sp, $sp, 28
sw $ra, -4($sp)
jal read
lw $ra, -4($sp)
addi $sp, $sp, -28
move $t1,$v0
sw $t1, 24($sp)
lw $t2, 24($sp)
move $t1, $t2
sw $t1, 28($sp)
addi $sp, $sp, 36
sw $ra, -4($sp)
jal read
lw $ra, -4($sp)
addi $sp, $sp, -36
move $t1,$v0
sw $t1, 32($sp)
lw $t2, 32($sp)
move $t1, $t2
sw $t1, 36($sp)
lw $t2, 4($sp)
move $t1, $t2
sw $t1, 40($sp)
lw $t2, 12($sp)
move $t1, $t2
sw $t1, 44($sp)
lw $t2, 20($sp)
move $t1, $t2
sw $t1, 48($sp)
lw $t2, 28($sp)
move $t1, $t2
sw $t1, 52($sp)
lw $t2, 36($sp)
move $t1, $t2
sw $t1, 56($sp)
lw $t1, 40($sp)
move $a0, $t1
lw $t1, 44($sp)
move $a1, $t1
lw $t1, 48($sp)
move $a2, $t1
lw $t1, 52($sp)
move $a3, $t1
lw $t1, 56($sp)
sw $t1, 64($sp)
addi $sp, $sp, 64
sw $ra, -4($sp)
jal myadd
lw $ra, -4($sp)
addi $sp, $sp, -64
move $t1,$v0
sw $t1, 60($sp)
lw $t2, 60($sp)
move $t1, $t2
sw $t1, 64($sp)
lw $t2, 64($sp)
move $t1, $t2
sw $t1, 68($sp)
lw $t1, 68($sp)
move $a0, $t1
addi $sp, $sp, 76
sw $ra, -4($sp)
jal write
lw $ra, -4($sp)
addi $sp, $sp, -76
lw $t1, 68($sp)
move $t1,$v0
sw $t1, 68($sp)
li $t1, 0
sw $t1, 72($sp)
lw $t1, 72($sp)
move $v0, $t1
jr $ra
