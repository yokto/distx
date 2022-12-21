// we need this for telling gdb about our symbols

// there are two possible ways. We can try to integrate into
// the what the native loader does. Or we can try to use the
// gdb jit interface

// use gdb jit way
//typedef enum
//{
//  JIT_NOACTION = 0,
//  JIT_REGISTER_FN,
//  JIT_UNREGISTER_FN
//} jit_actions_t;
//
//struct jit_code_entry
//{
//  struct jit_code_entry *next_entry;
//  struct jit_code_entry *prev_entry;
//  const char *symfile_addr;
//  uint64_t symfile_size;
//};
//
//struct jit_descriptor
//{
//  uint32_t version;
//  /* This type should be jit_actions_t, but we use uint32_t
//     to be explicit about the bitwidth.  */
//  uint32_t action_flag;
//  struct jit_code_entry *relevant_entry;
//  struct jit_code_entry *first_entry;
//};
//
///* GDB puts a breakpoint in this function.  */
//void __attribute__((noinline)) __jit_debug_register_code() { };
//
///* Make sure to specify the version statically, because the
//   debugger may check the version before we can set it.  */
//struct jit_descriptor __jit_debug_descriptor __attribute__ ((weak)) = { 1, 0, 0, 0 };

// use dynamic loader way
// see glibc / link.h
struct link_map {
    void* l_addr;          /* Difference between the address in the ELF
                                   file and the addresses in memory.  */
    char *l_name;               /* Absolute file name object was found in.  */
    void *l_ld;            /* Dynamic section of the shared object.  */
    struct link_map *l_next, *l_prev; /* Chain of loaded objects.  */
  };
struct r_debug
  {
    int r_version;              /* Version number for this protocol.  */
    struct link_map *r_map;     /* Head of the chain of loaded objects.  */
    void* r_brk;
    enum
      {
        /* This state value describes the mapping change taking place when
           the `r_brk' address is called.  */
        RT_CONSISTENT,          /* Mapping change is complete.  */
        RT_ADD,                 /* Beginning to add a new object.  */
        RT_DELETE               /* Beginning to remove an object mapping.  */
      } r_state;

    void* r_ldbase;        /* Base address the linker is loaded at.  */
  };
extern void _dl_debug_state (void) __attribute__ ((weak));
struct r_debug _r_debug __attribute__ ((weak)) = { 1, 0, 0, 0, 0 };
