## Local PE Injection

Khi thực hiện chạy 1 file exe, dll hay nói chung là PE, hệ điều hành sẽ thực hiện load file đó vào bộ nhớ, thực hiện 1 loạt  những quá trình cần thiết như load PE, fix IAT, fix relocation table,... Phần này sẽ hướng dẫn viết trình tải PE vào bộ nhớ chương trình hiện tại, sau đó thực hiện chạy PE từ đó.

