#include <efi.h>
#include <efilib.h>
#include <elf.h>


EFI_FILE *LoadFile(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);


typedef unsigned long long size_t;

typedef struct {

	void *base_address;
	size_t buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int pixels_per_sl;

} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {

	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;

} PSF1_HEADER;

typedef struct {

	PSF1_HEADER *psf1_header;
	void *glyph_buffer;

} PSF1_FONT;

PSF1_FONT *load_psf1_font(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	EFI_FILE *font = LoadFile(Directory, Path, ImageHandle, SystemTable);

	if(font == NULL) {
		return NULL;
	}

	PSF1_HEADER *fontheader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontheader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontheader);

	if(fontheader->magic[0] != PSF1_MAGIC0 || fontheader->magic[1] != PSF1_MAGIC1) {
		return NULL;
	}

	UINTN glyphbuffersize;
	if(fontheader->mode == 1) { //512
		glyphbuffersize = fontheader->charsize * 512;
	} else {
		glyphbuffersize  = fontheader->charsize * 256;
	}

	void *glyphbuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphbuffersize, (void**)&glyphbuffer);
		font->Read(font, &glyphbuffersize, glyphbuffer);
	}

	PSF1_FONT *finishedfont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedfont);
	finishedfont->psf1_header = fontheader;
	finishedfont->glyph_buffer = glyphbuffer;


	return finishedfont;
}

Framebuffer frame_buffer;


Framebuffer  *initialize_gop() {

	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_STATUS status;


	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);

	if(EFI_ERROR(status)) {
		Print(L"Unable to locate GOP\n\r");
		return NULL;
	} else {
		Print(L"GOP located\n\r");
	}

	frame_buffer.base_address = (void*)gop->Mode->FrameBufferBase;
	frame_buffer.buffer_size = gop->Mode->FrameBufferSize;
	frame_buffer.width = gop->Mode->Info->HorizontalResolution;
	frame_buffer.height = gop->Mode->Info->VerticalResolution;
	frame_buffer.pixels_per_sl = gop->Mode->Info->PixelsPerScanLine;

	return &frame_buffer;
}



EFI_FILE *LoadFile(EFI_FILE *Directory, CHAR16 *Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	EFI_FILE *loaded_file;
	
	EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system;
	SystemTable->BootServices->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_system);

	if(Directory == NULL) {
		file_system->OpenVolume(file_system, &Directory); //Set to root of file system if null
	}

	EFI_STATUS s = Directory->Open(Directory, &loaded_file, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if(s != EFI_SUCCESS) {
		return NULL;
	}

	return loaded_file;
}


int memcmp(const void* aptr, const void* bptr, size_t n) {

	const unsigned char *a = aptr;
	const unsigned char *b = bptr;

	for(size_t i = 0; i < n; i++) {
		if(a[i] < b[i]) {
			return -1;
		} else if(a[i] > b[i]){
			return 1;
		}
	}

	return 0;

}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	InitializeLib(ImageHandle, SystemTable);

	EFI_FILE *Kernel;

	if((Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable)) == NULL) {
		Print(L"Could not load kernel object\n\r");
	} else {
		Print(L"Kernel object loaded successfully\n\r");
	}

	
	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO *FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	) {
		Print(L"Kernel format is incorrect\n\r");
	} else {
		Print(L"Kernel header verified\n\r");
	}

	Elf64_Phdr *phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr *phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch (phdr->p_type)
		{
		case PT_LOAD:
			int pages = (phdr->p_memsz + 0x1000  - 1) / 0x1000;
			Elf64_Addr segment = phdr->p_paddr;
			SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
			Kernel->SetPosition(Kernel, phdr->p_offset);
			UINTN size = phdr->p_filesz;
			Kernel->Read(Kernel, &size, (void*)segment);
			break;
		}
	}

	Print(L"Kernel is loaded\n\r");

	Framebuffer *new_buffer = initialize_gop(); //graphics output protocol

	void (*KernelStart)(Framebuffer*, PSF1_FONT*) = ((__attribute__((sysv_abi)) void (*)(Framebuffer*, PSF1_FONT*) ) header.e_entry);

	PSF1_FONT *newfont = load_psf1_font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);

	if(newfont == NULL) {
		Print(L"Font is not valid or is not found\n\r");
	} else {
		Print(L"Font found at %s loaded\n\r", L"zap-light16.psf");
		Print(L"Char size = %d\n\r", newfont->psf1_header->charsize);
	}

	Print(L"Base: 0x%x\n\rSize: 0x%x\n\rWidth: %d\n\rHeight: %d\n\rPixels per scanline: %d\n\r",
			new_buffer->base_address, new_buffer->buffer_size, new_buffer->width, new_buffer->height,
		new_buffer->pixels_per_sl);




	KernelStart(new_buffer, newfont);

	return EFI_SUCCESS; // Exit the UEFI application
}
