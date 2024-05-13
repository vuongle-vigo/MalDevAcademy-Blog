## Module Details and Binary Signature Modify

### Module Details

Để hiểu 1 cách đơn giản, hãy cùng xem 2 file `DllMessageBox.dll` và `kernel32.dll` ở dưới:

![details](images/details.png)

Phần details của `kernel32.dll` tất nhiên sẽ chứa những thông tin mang phần uy tín hơn so với `DllMessageBox.dll`. Phần này sẽ hướng dẫn cài đặt những thông tin này vào file dll, exe của chúng ta.

***Method 1: Resource Hacker***

Đầu tiên mở file cần sửa đổi với `Resource Hacker`, chọn **Action => Add from Resource file**

![addrs1](images/addrs1.png)

Tìm đến file cần lấy details, ở đây là `kernel32.dll`, tick vào `VersionInfo` và add, save file sẽ có thông tin cần thêm:

![verinfo1](images/verinfo1.png)

***Method 2: Add resource VS2022***

Mở dll, exe với VS2022 (hoặc các phiên bản khác), sau đó add resource (Ở phần resource file), ở đây đang đặt tên file là `metadata.rc`. Sau đó thực hiện chỉnh sửa nội dung file như sau:

```rc
1 VERSIONINFO
FILEVERSION 112, 0, 5615, 88 // File version separated by commas
PRODUCTVERSION 1, 0, 0, 0
FILEFLAGSMASK 0x0L
#ifdef _DEBUG
FILEFLAGS 0x1L
#else
FILEFLAGS 0x0L
#endif
FILEOS 0x0L
FILETYPE 0x0L
FILESUBTYPE 0x0L
BEGIN
BLOCK "StringFileInfo"
BEGIN
BLOCK "040904B0"
BEGIN
// Modify the values below
VALUE "CompanyName", "Google LLC."
VALUE "FileDescription", "Google Chrome"
VALUE "InternalName", "Chrome"
VALUE "LegalCopyright", "Copyright 2023 Google LLC."
VALUE "OriginalFilename", "chrome.exe"
VALUE "ProductName", "Google Chrome"
VALUE "ProductVersion", "112.0.5615.86"
END
END
BLOCK "VarFileInfo"
BEGIN
VALUE "Translation", 0x409, 1200
END
END
```

Build file và kết quả như dưới đây:

![result1](images/result1.png)

### Binary Signature Modify



