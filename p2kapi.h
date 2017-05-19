#ifndef __TRC_P2K_API_HEADER_H__
#define __TRC_P2K_API_HEADER_H__

/*!
\file trcp2kapi.h
Header file designed for P2K API Version 1.1 Build 2 by Vilko.
Based on original API readme file.
For usage with C/C++.

Version 1.1.0
By Jacek "TeMPOraL" Zlydach.
Copyleft (<C) 2006 - Eternity, Temporal Research Centre

Last revised: 11.06.2007 [dd/mm/yyyy]

Well, it's not really a correct Doxygen code here, but it is still understandable for humans ;).

Changes:
	* 1.1.0 - Added DLL function names in documentation :)

*/

/*
Following are the names of functions that can be found in P2KApi DLL

	File_Close
	File_Count
	File_Create
	File_Delete
	File_List
	File_Read
	File_SetPointer
	File_VolInfo
	File_Write
	P2K_GetStatus
	P2K_Init
	P2K_Restart
	Seem_Read
	Seem_Write

*/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

/*! \name Main constants. */
/*@{*/
#define TRC_P2K_SUCCESS 0
#define TRC_P2K_ERROR -1

#define TRC_P2K_FILEATTR_DEFAULT 0
#define TRC_P2K_FILEATTR_READONLY 1
#define TRC_P2K_FILEATTR_HIDDEN 2
#define TRC_P2K_FILEATTR_SYSTEM 4

#define TRC_P2K_FILEPOS_BEG 0
#define TRC_P2K_FILEPOS_CUR 1
#define TRC_P2K_FILEPOS_END 2
/*@}*/

/*! \name Base size typedefs.*/
/*@{*/
typedef unsigned char TRC_P2K_BYTE;
typedef unsigned short TRC_P2K_WORD;
typedef unsigned int TRC_P2K_DWORD;

typedef int TRC_P2K_INTEGER_TYPE;
typedef char* TRC_P2K_LPSTR;

typedef void* TRC_P2K_POINTER;
/*@}*/

//================================

/*! \name P2K Functions */
/*@{*/

/*!	\brief Phone status monitor callback procedure.
	\param Reason for calling the function - 1 if phone detected (connect),
	0 if not detected (disconnect).
*/
typedef void (*TRC_P2K_STATUS_FUNCTION)(TRC_P2K_INTEGER_TYPE);

/*!	\brief (DLL Name: P2K_Init) Initialize P2K API, start up phone status monitor.
	\param Pointer to a procedure that will be called on phone detect/disconnect.
	If NULL is passed, no function will be called.
	\return Returns 0 on initialization success, 1 on failure.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_Init)(TRC_P2K_STATUS_FUNCTION);

/*!	\brief (DLL Name: P2K_Restart) Restart phone.

	NOTE
	It is not yet known whether or not this function returns anything.
	Also what happens if it is called without previous call to trcP2K_Init is not yet known.
*/
typedef void (*trcP2K_Restart)();

/*!
	\brief (DLL Name: P2K_GetStatus) Get connection status.
	\return Returns 0 if phone not found, 1 if phone found.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_GetStatus)();

/*@}*/

//================================

/*! \name File functions */
/*@{*/

/*!	\brief (DLL Name: File_Create) Create a new file or open an existing one.
	\param fileName - pointer to string containing a file name (in case of nested filesystem - full path with directories).
	\param attribute - new attribute for a created/opened file
		Attribute is a combination of 3 bits:
		bit0 - readonly (TRC_P2K_FILEATTR_READONLY)
		bit1 - hidden (TRC_P2K_FILEATTR_HIDDEN)
		bit2 - system (TRC_P2K_FILEATTR_SYSTEM)
	\return Returns 0 if opening was successful or -1 on error.

	NOTE
	Phone not allows to open more than 1 file simultaneously.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Create)(TRC_P2K_LPSTR fileName, TRC_P2K_BYTE attribute);

/*!	\brief (DLL Name: File_Close) Closes previously opened file.
	\return Returns 0 if file was successfuly closed, -1 otherwise.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Close)();

/*!	\brief (DLL Name: File_Read) Reads data from currently opened file.
	\param buffer - pointer to a buffer in which read data will be stored.
	\param size - size in bytes of data block to read.
		Size of output buffer must be greater or equal to this value.
	\return Returns 0 if reading operation was successful, -1 on error.

	NOTE
	This function reads data either from the beginning of the file, or the position
	specified by File_SetPointer function. Whether this function changes the state
	of the file pointer or not is yet unknown.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Read)(TRC_P2K_POINTER buffer, TRC_P2K_DWORD size);

/*!	\brief (DLL Name: File_Write) Writes data to the currently opened file.
	\param buffer - input buffer containing data to store on the phone.
	\param size - number of bytes to be read from the input buffer and stored on the phone.
	\return Returns 0 on success, -1 on error.

	NOTE
	This function does not check if there is any free space for writing.
	Also the phone itself needs some free space for temporary files.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Write)(TRC_P2K_POINTER buffer, TRC_P2K_DWORD Size);

/*!	\brief (DLL Name: File_SetPointer) Set read/write file pointer.
	\param offset - new position of the pointer.
	\param moveMethod - describes how to set the pointer:
		0 - offset from the beginning or the file (TRC_P2K_FILEPOS_BEG)
		1 - offset from current position of pointer (TRC_P2K_FILEPOS_CUR)
		2 - offset from the end of the file (TRC_P2K_FILEPOS_END)
	\return Returns 0 on success, -1 on error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_SetPointer)(TRC_P2K_DWORD offset, TRC_P2K_BYTE moveMethod);

/*!	\brief (DLL Name: File_Delete) Delete a file.
	\param fileName - pointer to string containing a file name (in case of nested filesystem - full path with directories).
	\return Returns 0 on success, -1 on error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Delete)(TRC_P2K_LPSTR fileName);

/*!	\brief (DLL Name: File_VolInfo) Receive information about free space and volume name.
	\param buffer - pointer to a buffer for storing the volume name (must be >= 128 bytes)
	\return Returns free space size in bytes, or -1 on error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_VolInfo)(TRC_P2K_POINTER buffer);

/*!	\brief (DLL Name: File_Count) Get file count.
	\return Returns file count, or -1 if error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_Count)();

/*!	\brief (DLL Name: File_List) Get file list.
	\param buffer - output buffer to store the file list.
		File list is saved as:
			dd file size (dword, 4 bytes)
			dw attribute 1 (word, 2 bytes)
			dw attribute 2 (word, 2 bytes)
		string of 1F8h length - file name.
		Record size per 1 file is 200h bytes
		Records count is equivalent to files count in phone (result of File_Count() function)
		According to this, size of buffer for files list must be File_Count*200h
	\param count - File count to read (result of File_Count() function).
	\return Returns a maximum length of file name for current phone model or -1 on error.

	NOTE
	It is not a good idea to call it with File_Count() as a second parameter, as the File_Count()
	may return -1 as an error, and library behaviour for this situation is unknown.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_File_List)(TRC_P2K_POINTER buffer, TRC_P2K_DWORD count);

/*@}*/

//================================

/*! \name Seem functions */
/*@{*/

/*!	\brief (DLL Name: Seem_Read) Read content of a seem cell.
	\param seem - seem cell number.
	\param record - seem record number.
	\param startOffset - how many bytes to skip (0 - read from the very beginning).
	\param bytes - how many bytes to read (0 - read to the end).
	\param data - Output buffer to store read data. If bytes is 0, buffer size must be >= 2710h.
	\return Returns count of read bytes, or -1 on error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_Seem_Read)(TRC_P2K_WORD seem, TRC_P2K_WORD record, TRC_P2K_WORD startOffset, TRC_P2K_WORD bytes, TRC_P2K_POINTER data);

/*!	\brief (DLL Name: Seem_Write) Write data to a seem cell.
	\param seem - seem cell number.
	\param record - seem record number.
	\param startOffset - where to start (in bytes), 0 - from cell beginning)
	\param bytes - bytes count from writing (0 - from the beginning to the end of a cell?)
	\param data - pointer to a buffer containing data to be written.
	\return Returns 0 after a successful writing, or -1 on error.
*/
typedef TRC_P2K_INTEGER_TYPE (*trcP2K_Seem_Write)(TRC_P2K_WORD seem, TRC_P2K_WORD record, TRC_P2K_WORD startOffset, TRC_P2K_WORD bytes, TRC_P2K_POINTER data);
/*@}*/

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*__TRC_P2K_API_HEADER_H__*/
