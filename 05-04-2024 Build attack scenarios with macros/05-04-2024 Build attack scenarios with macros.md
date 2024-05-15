## Build attack scenarios with macro


### Giới thiệu

Module tập trung giới thiệu các kịch bản tấn công bằng macro excel (word sử dụng tương tự), cùng với các cơ chế chống macro mà microsoft đã áp dụng gần đây.

### Microsoft has blocked macros (MOTW)

Đây là cơ chế mới được microsoft áp dụng cho tất cả các tập tin office được tải xuống từ internet, các file này sẽ được đánh dấu là untrusted, bất kể là file có chứa macro hay không, điều này sẽ ngăn chặn việc thực thi macro và yêu cầu người dùng bật macro bằng tay, như hình dưới:

![untrusted](images/untrusted.png)

Như vậy, khi mở file có chứa macro, macro sẽ bị chặn và cảnh báo sau sẽ được hiển thị:

![blocked](images/blocked.png)

***HOW TO BYPASS***

Có 2 cách để bypass được việc này, 1 là có chứng chỉ cho tài liệu office được tải về, bài viết không nói về cách gán chứng chỉ cho tài liệu nên có thể tìm hiểu thêm ở google, tuy nhiên giá của chứng chỉ mua ở các chợ đen khá đắt, khoảng ~500$.

Cách thứ 2 đó là có 1 số định dạng file zip có thể khiến cho microsoft không thể quét được các file bên trong để đánh dấu chúng. Qua tìm hiểu thì hiện tại mình chỉ phát hiện được 1 định dạng file có thể bypass được việc này, tuy nhiên vẫn phải enable macro để có thể chạy được như trước đây.

### Protected View

Cơ chế này tạo ra 1 file temp để cho người xem xem, cần enable để vào chế độ chỉnh sửa. Nguyên nhân là file được đặt ở những vị trí "untrusted" trên máy tính. Qua thử nghiệm thì có vẻ nếu như file đặt ở `Documents` thì có thể bypass được chế độ này.

![protectedview](images/protectedview.png)

Dù sao cũng có thể bỏ qua 1 bước ấn enable của người dùng nên bypass chế độ này khá là cần thiết, kết hợp với phần 1 có thể sử dụng để unzip file vào `Documents` để chạy file.

### Kịch bản 1: Sử dụng lnk file để chạy macro

Một kịch bản hacker thường sử dụng, đó là dùng file lnk. Chỉnh sửa phần target trong file lnk có thể cho phép thực thi các câu lệnh khi click mở file. Tuy nhiên hiện tại số ký tự được sử dụng đang bị giới hạn khoảng ~260 ký tự. Vấn đề thứ 2 là nếu kịch bản gửi kèm theo nhiều file thì cần phải zip, do đó victim cần phải unzip, sau đó click vào file .lnk thì mới chạy được file. Ngoài ra có thể viết mã trong lnk để tự động unzip và thực thi mà không cần người dùng tự unzip.

![lnk](images/lnk.png)

***Enable macros***

Ngoài ra, macro mặc định được tắt, vì vậy để thực thi cần enable macro, kết hợp file lnk có thể chạy 1 file bat với mã dưới đây để bật macro từ cmd.

```bat
reg add "HKEY_CURRENT_USER\Software\Microsoft\Office\16.0\Excel\Security" /v VBAWarnings /t REG_DWORD /d 1 /f
```

***Bypass MOTW***

Kết hợp với việc chạy file bat, ta có thể thay đổi định dạng file excel thành 1 định dạng khác, như vậy sẽ không thể bị MOTW quét. Sau đó khi chạy file bat, thực hiện khôi phục định dạng cũ của excel.

```bat
ren test.txt test.xlsx
```
Như vậy chỉ cần 1 click vào .lnk, có thể bypass. Tuy nhiên kịch bản hơi khó thực hiện vì liên quan tới việc zip và unzip, yêu cầu phải lừa được victim thực hiện theo từng bước.


### Kịch bản 2: Sử dụng winrar + excel file type bypass MOTW

Như đã nói ở trước, việc này yêu cầu phải tìm được định dạng file zip thích hợp, sau đó mở file excel sẽ cho phép bypass được MOTW.

Cảnh báo đầu tiên là định dạng file sai, ấn Yes để có thể xem được file.

![slk](images/slk.png)

Sau đó chỉ cần enable macro nếu cần để thực thi macro mà không bị block như trước.

Dưới đây là 1 link khá hay cho các bạn muốn sử dụng các mã VBA để thực thi, dump, download dll hay exe qua macro.

https://github.com/S3cur3Th1sSh1t/OffensiveVBA