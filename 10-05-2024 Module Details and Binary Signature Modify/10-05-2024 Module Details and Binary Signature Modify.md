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

Phần này sẽ hướng dẫn tạo signature giả, chủ yếu dùng để tăng sự tin tưởng của người dùng, signature không thật sự được ký nên sẽ không có tác dụng bypass AV. Các tool được sử dụng có thể tìm thấy ở thư mục tools upload kèm theo, hoặc tại `C:\Program Files (x86)\Windows Kits\...`

Dưới đây là các lệnh để sự dụng tạo signature trên `DllMessageBox.dll`:

```bat
MakeCert.exe -r -pe -n "CN = Microsoft Root Certificate Authority 2010,O = Microsoft Corporation,L = Redmond,S = Washington,C = US" -ss CA -sr CurrentUser -a sha256 -cy authority -sky signature -sv CA.pvk CA.cer


MakeCert.exe -pe -n "CN=Microsoft Windows Production PCA 2011,O = Microsoft Corporation,L = Redmond,S = Washington,C = US" -a sha256 -cy end -sky signature -eku 1.3.6.1.5.5.7.3.3,1.3.6.1.4.1.311.10.3.24,1.3.6.1.4.1.311.10.3.6 -ic CA.cer -iv CA.pvk -sv SPC.pvk SPC.cer

pvk2pfx -pvk SPC.pvk -spc SPC.cer -pfx SPC.pfx

sigtool.exe sign /v /f SPC.pfx DllMessageBox.dll
```

Và đây là kết quả:

![result2](images/result2.png)