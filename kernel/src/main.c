#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <drivers/serial.h>
#include <klib/logging.h>
#include <limine.h>
#include <mem/heap.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <panic.h>
#include <stdint.h>
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
        .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
        .id = LIMINE_MEMMAP_REQUEST_ID,
        .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
        .id = LIMINE_HHDM_REQUEST_ID,
        .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request exec_request = {
        .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
        .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void kernel_main(void) {
        if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
                panic("limine base revision %d is not supported", limine_base_revision);

        if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
                panic("no framebuffer found");

        if (serial_init() < 0)
                panic("failed to initialize serial port");
        info("initialized serial driver");

        gdt_init();
        info("installed gdt");

        idt_init();
        info("installed idt");

        pmm_init(memmap_request.response);
        info("initialized pmm");

        vmm_init(hhdm_request.response, exec_request.response, memmap_request.response);
        info("initialized vmm");

        heap_init();
        info("initialized heap");

        panic("nothing to do");
}