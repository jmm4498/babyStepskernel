typedef unsigned long long UINTN;
typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

typedef struct {

  UINT32 Type;
  UINT32 Pad;
  UINT64 PhysicalStart;
  UINT64 VirtualStart;
  UINT64 NumberOfPages;

} EFI_MEMORY_DESCRIPTOR;
