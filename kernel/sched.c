#include "hardware.h"
#include "util.h"
#include "lapic_timer.h"

#define STACK_SIZE 1024
#define TASK_NUM 3

char stack0 [STACK_SIZE];
char stack1 [STACK_SIZE];
char stack2 [STACK_SIZE];

static void task0() {
    while (1) {
        puts("hello from task0\n");
        volatile int i = 100000000;
        while (i--);
    }
}

static void task1() {
    while (1) {
        puts("hello from task1\n");
        volatile int i = 100000000;
        while (i--);
    }
}

static void task2() {
    while (1) {
        puts("hello from task2\n");
        volatile int i = 100000000;
        while (i--);
    }
}

struct Task {
    unsigned long long sp;
};

struct Task tasks[TASK_NUM];
unsigned int current_task;

static void init_task(int idx, unsigned char *stack_bottom, unsigned long long rip) {
    unsigned long long *sp = (unsigned long long *)stack_bottom;
    unsigned long long reg64;

    /* failed codes */
    // asm volatile (
    //         "mov %%rsp, %0\n"
    //         "mov %1, %%rsp\n"
    //         "mov %%ss, %%rax\n"
    //         "push %%rax\n"
    //         "push %1\n"
    //         "pushfq\n"
    //         "mov %%cs, %%rax\n"
    //         "push %%rax\n" : "=r"(reg64) : "m"(sp)
    //         );
    // asm volatile ("push %0\n" :: "m"(rip));
    // asm volatile ("mov %0, %%rsp\n" :: "m"(reg64));

    /* alternative codes */
    unsigned long long ss;
    asm volatile("mov %%ss, %0" : "=r"(ss));
    *(sp-1) = ss;
    *(sp-2) = (unsigned long long)sp;
    sp -= 2;
    asm volatile (
            "mov %%rsp, %0\n"
            "mov %1, %%rsp\n"
            "pushfq\n" : "=r"(reg64) : "m"(sp)
            );
    sp--;
    asm volatile ("mov %0, %%rsp\n" :: "m"(reg64));
    unsigned long long cs;
    asm volatile ("mov %%cs, %0" : "=r"(cs));
    *(sp-1) = cs;
    *(sp-2) = rip;

    sp -= 17;
    tasks[idx].sp = (unsigned long long)sp;
}

void init_tasks() {
    init_task(1, (unsigned char *)stack1+STACK_SIZE, (unsigned long long)task1);
    init_task(2, (unsigned char *)stack2+STACK_SIZE, (unsigned long long)task2);
    unsigned long long sp0 = (unsigned long long)stack0+STACK_SIZE;
    asm volatile ("mov %0, %%rsp\n" :: "m"(sp0));
    task0();
}

void schedule(unsigned long long sp) {
    tasks[current_task].sp = sp;
    current_task = (++current_task)%3;
    lapic_set_eoi();

    asm volatile (
            "mov %0, %%rsp\n"
            "pop %%r15\n"
            "pop %%r14\n"
            "pop %%r13\n"
            "pop %%r12\n"
            "pop %%r11\n"
            "pop %%r10\n"
            "pop %%r9\n"
            "pop %%r8\n"
            "pop %%rdi\n"
            "pop %%rsi\n"
            "pop %%rbp\n"
            "pop %%rbx\n"
            "pop %%rdx\n"
            "pop %%rcx\n"
            "pop %%rax\n"
            "iretq\n" :: "m"(tasks[current_task].sp)
            );
}
