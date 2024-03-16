## API Hooking

API Hooking là việc chuyển hướng API gốc đến một function khác để thực thi. Điều này đạt được bằng cách đọc và sửa đổi mã của API, chuyển hướng nó đến mã cần thực thi và sửa lại ban đầu. API Hooking thường được sử dụng trong việc phân tích và gỡ lỗi, phát hiện phầm mềm độc hại, tuy nhiên trong việc phát triển malware nó cũng có 1 số chức năng như sau:

- Thu thập thông tin nhạy cảm (ví dụ: Credentials)
- Sửa đổi hoặc chặn các lệnh gọi hàm nhằm mục đích xấu
- Bỏ qua các biện pháp bảo mật bằng cách thay đổi cách hoạt động của hệ điều hành (AMSI, ETW,...)

### Cơ chế hoạt động

Dưới đây là sơ đồ cách thay đổi hành vi của 1 API:
![apihooking](images/apihooking.png)

Sau khi thực hiện sửa đổi mã của API, thực thi mã của riêng chúng ta, cần sửa lại mã như ban đầu, đồng thời có thể gọi API ở trong hàm độc hại, nhằm mục đích thực hiện API ban đầu được gọi, tránh gây chú ý.
Ngoài ra, tham số của hàm tự tạo của chúng ta cũng cần phải giống với API được gọi, nhằm truyền nó vào đợt gọi API kết tiếp.

### Code




