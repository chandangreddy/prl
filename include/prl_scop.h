#ifndef PRL_SCOP_H
#define PRL_SCOP_H

#include <stddef.h>
#include <stdbool.h>

#include "prl_mem.h"

#if defined(__cplusplus)
extern "C" {
#endif

//TODO: remove???
struct prl_scop_struct;
typedef struct prl_scop_struct *prl_scop;

struct prl_scop_inst_struct;
typedef struct prl_scop_inst_struct *prl_scop_instance;

struct prl_program_struct;
typedef struct prl_program_struct *prl_program;

struct prl_kernel_struct;
typedef struct prl_kernel_struct *prl_kernel;

enum prl_kernel_call_arg_type {
    prl_kernel_call_arg_mem,
    prl_kernel_call_arg_value
};

struct prl_kernel_call_arg {
    enum prl_kernel_call_arg_type type;
    union {
        prl_mem mem;
        struct {
            void *data;
            size_t size;
        };
    };
};

prl_scop_instance prl_scop_enter(prl_scop *scop); // fixed
void prl_scop_leave(prl_scop_instance scop);      // fixed

//TODO: rename prl_scop_opencl_*
void prl_scop_program_from_file(prl_scop_instance scop, prl_program *program, const char *filename, const char *build_options); //TODO: Independent of SCOPinstance
void prl_scop_program_from_str(prl_scop_instance scop, prl_program *program, const char *str, size_t str_size, const char *build_options);
void prl_scop_init_kernel(prl_scop_instance scop, prl_kernel *kernel, prl_program program, const char *kernelname);

#if __STDC__ >= 199901L
// C99
void prl_scop_call(prl_scop_instance scopinst, prl_kernel kernel, int grid_dims, size_t grid_size[static const restrict grid_dims], int block_dims, size_t block_size[static const restrict block_dims], size_t n_args, struct prl_kernel_call_arg args[static const restrict n_args]);
#else
// C89 or C++
void prl_scop_call(prl_scop_instance scopinst, prl_kernel kernel, int grid_dims, size_t grid_size[], int block_dims, size_t block_size[], size_t n_args, struct prl_kernel_call_arg args[]);
#endif

// Scenarios to cover:
//   host+malloc    <-> device+allocation_per_scop
//   host+prl_alloc <-> device+prl_alloc
//                      device+
prl_mem prl_scop_get_mem(prl_scop_instance scop, void *host_mem /*might be result of prl_alloc*/, size_t size, const char *name);
//void prl_scop_mem_release(prl_mem mem);

void prl_scop_host_to_device(prl_scop_instance scop, prl_mem mem);
void prl_scop_device_to_host(prl_scop_instance scop, prl_mem mem);
//void prl_scop_host_wait(prl_scop_instance scop, prl_mem mem);

#if defined(__cplusplus)
}
#endif

#endif /* PRL_SCOP_H */
