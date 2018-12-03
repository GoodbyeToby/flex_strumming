.cpu cortex-m0
.thumb
.syntax unified

.global nanowait
nanowait:
	subs r0,#83
	bge nanowait
	bx lr
