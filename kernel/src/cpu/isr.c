#include <klib/logging.h>
#include <klib/print.h>
#include <panic.h>
#include <stdint.h>

struct int_frame {
	uint64_t cr2, cr3;
	uint64_t rax, rbx, rcx, rdx;
	uint64_t rsi, rdi;
	uint64_t rbp;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t err_type, err_code;
	uint64_t rip;
	uint64_t cs;
	uint64_t flags;
	uint64_t rsp;
	uint64_t ss;
};

static const char *err_type_str_table[] = {
        [0]     = "division by zero",
        [13]    = "general protection fault",
        [14]    = "page fault"
};

static void int_frame_dump(struct int_frame *frame)
{
        printf("-------------------- Registers dump ---------------------\r\n");
        printf(
                "CR2 = 0x%x CR3=0x%x\r\n"
                "RAX = 0x%x RBX = 0x%x RCX = 0x%x RDX = 0x%x\r\n"
                "RSI = 0x%x RDI = 0x%x\r\n"
                "RBP = 0x%x\r\n"
                "R8 = 0x%x R9 = 0x%x R10 = 0x%x R11 = 0x%x R12 = 0x%x R13 = 0x%x R14 = 0x%x R15 = 0x%x\r\n"
                "RIP = 0x%x\r\n"
                "CS = 0x%x\r\n"
                "FLAGS = 0x%x\r\n"
                "RSP = 0x%x\r\n"
                "SS = 0x%x\r\n",
                frame->cr2, frame->cr3,
                frame->rax, frame->rbx, frame->rcx, frame->rdx,
                frame->rsi, frame->rdi,
                frame->rbp,
                frame->r8, frame->r9, frame->r10, frame->r11, frame->r12, frame->r13, frame->r14, frame->r15,
                frame->rip,
                frame->cs,
                frame->flags,
                frame->rsp,
                frame->ss
        );
}

void isr_handle(struct int_frame *frame)
{
        fatal("CPU exception #%d with error code %d", frame->err_type, frame->err_code);
        int_frame_dump(frame);
        panic(err_type_str_table[frame->err_type]);
}