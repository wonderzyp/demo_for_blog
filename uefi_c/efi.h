#ifndef _EFI_H_
#define _EFI_H_

#define EFI_SUCCESS	0
#define EFI_ERROR	0x8000000000000000
#define EFI_UNSUPPORTED	(EFI_ERROR | 3)

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL	0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL		0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL		0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER	0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER		0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE		0x00000020

struct EFI_INPUT_KEY {
	unsigned short ScanCode;
	unsigned short UnicodeChar;
};


struct EFI_DEVICE_PATH_PROTOCOL {
	unsigned char Type;
	unsigned char SubType;
	unsigned char Length[2];
};

struct EFI_GUID {
	unsigned int Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
};

struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {
	unsigned long long _buf;

	unsigned short *(*ConvertDevicePathToText)(
		const struct EFI_DEVICE_PATH_PROTOCOL* DeviceNode,
		unsigned char DisplayOnly,
		unsigned char AllowShortcuts);
};

struct EFI_SYSTEM_TABLE {
	char _buf1[44];
	struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
		unsigned long long _buf;
		unsigned long long (*ReadKeyStroke)(
			struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
			struct EFI_INPUT_KEY *Key);
		void *WaitForKey;
	} *ConIn;
	unsigned long long _buf2;
	struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
		unsigned long long _buf;
		unsigned long long (*OutputString)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
			unsigned short *String);
		unsigned long long (*TestString)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
			unsigned short *String);
		unsigned long long (*QueryMode)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
			unsigned long long ModeNumber,
			unsigned long long *Columns,
			unsigned long long *Rows);
		unsigned long long (*SetMode)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
			unsigned long long ModeNumber);
		unsigned long long (*SetAttribute)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
			unsigned long long Attribute);
		unsigned long long (*ClearScreen)(
			struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
		unsigned long long _buf4[2];
		struct SIMPLE_TEXT_OUTPUT_MODE {
			int MaxMode;
			int Mode;
			int Attribute;
			int CursorColumn;
			int CursorRow;
			unsigned char CursorVisible;
		} *Mode;
	} *ConOut;
	unsigned long long _buf3[3];
	struct EFI_BOOT_SERVICES {
		char _buf1[24];

		//
		// Task Priority Services
		//
		unsigned long long _buf2[2];

		//
		// Memory Services
		//
		unsigned long long _buf3[5];

		//
		// Event & Timer Services
		//
		unsigned long long _buf4[2];
		unsigned long long (*WaitForEvent)(
			unsigned long long NumberOfEvents,
			void **Event,
			unsigned long long *Index);
		unsigned long long _buf4_2[3];

		//
		// Protocol Handler Services
		//
		unsigned long long _buf5[9];

		//
		// Image Services
		//
		unsigned long long (*LoadImage)(
			unsigned char BootPolicy,
			void *ParentImageHandle,
			struct EFI_DEVICE_PATH_PROTOCOL *DevicePath,
			void *SourceBuffer,
			unsigned long long SourceSize,
			void **ImageHandle);
		unsigned long long (*StartImage)(
			void *ImageHandle,
			unsigned long long *ExitDataSize,
			unsigned short **ExitData);
		unsigned long long _buf6[3];

		//
		// Miscellaneous Services
		//
		unsigned long long _buf7[2];
		unsigned long long (*SetWatchdogTimer)(
			unsigned long long Timeout,
			unsigned long long WatchdogCode,
			unsigned long long DataSize,
			unsigned short *WatchdogData);

		//
		// DriverSupport Services
		//
		unsigned long long _buf8[2];

		//
		// Open and Close Protocol Services
		//
		unsigned long long (*OpenProtocol)(
			void *Handle,
			struct EFI_GUID *Protocol,
			void **Interface,
			void *AgentHandle,
			void *ControllerHandle,
			unsigned int Attributes);
		unsigned long long _buf9[2];

		//
		// Library Services
		//
		unsigned long long _buf10[2];
		unsigned long long (*LocateProtocol)(
			struct EFI_GUID *Protocol,
			void *Registration,
			void **Interface);
		unsigned long long _buf10_2[2];

		//
		// 32-bit CRC Services
		//
		unsigned long long _buf11;

		//
		// Miscellaneous Services
		//
		unsigned long long _buf12[3];
	} *BootServices;
};

enum EFI_MEMORY_TYPE {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiMaxMemoryType
};

struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {
	struct EFI_DEVICE_PATH_PROTOCOL *(*ConvertTextToDeviceNode) (
		const unsigned short *TextDeviceNode);
	struct EFI_DEVICE_PATH_PROTOCOL *(*ConvertTextToDevicePath) (
		const unsigned short *TextDevicePath);
};

struct EFI_LOADED_IMAGE_PROTOCOL {
	unsigned int Revision;
	void *ParentHandle;
	struct EFI_SYSTEM_TABLE *SystemTable;
	// Source location of the image
	void *DeviceHandle;
	struct EFI_DEVICE_PATH_PROTOCOL *FilePath;
	void *Reserved;
	// Image’s load options
	unsigned int LoadOptionsSize;
	void *LoadOptions;
	// Location where image was loaded
	void *ImageBase;
	unsigned long long ImageSize;
	enum EFI_MEMORY_TYPE ImageCodeType;
	enum EFI_MEMORY_TYPE ImageDataType;
	unsigned long long (*Unload)(void *ImageHandle);
};

struct EFI_DEVICE_PATH_UTILITIES_PROTOCOL {
	unsigned long long _buf[3];
	struct EFI_DEVICE_PATH_PROTOCOL *(*AppendDeviceNode)(
		const struct EFI_DEVICE_PATH_PROTOCOL *DevicePath,
		const struct EFI_DEVICE_PATH_PROTOCOL *DeviceNode);
};


extern struct EFI_SYSTEM_TABLE *ST;
extern struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern struct EFI_SIMPLE_POINTER_PROTOCOL *SPP;
extern struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
extern struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *STIEP;
extern struct EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
extern struct EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
extern struct EFI_DEVICE_PATH_UTILITIES_PROTOCOL *DPUP;
extern struct EFI_GUID lip_guid;
extern struct EFI_GUID dpp_guid;


void efi_init(struct EFI_SYSTEM_TABLE *SystemTable);

#endif