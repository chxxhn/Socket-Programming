# 🖧 Socket-Programming Project #1

## 📌 프로젝트 개요
이 프로젝트는 **컴퓨터 네트워크 소켓 프로그래밍**을 실습하기 위한 **서버-클라이언트** 통신 프로그램입니다.  
**AF_UNIX 및 AF_INET 도메인 소켓**을 사용하여 메시지 송수신 기능을 구현하였으며, **동기형 1:1 채팅 및 멀티서비스 서버**를 포함합니다.

---

## 📂 프로젝트 구조


---

## 🛠 사용 기술
- **프로그래밍 언어**: C
- **소켓 프로그래밍**:
  - `AF_UNIX` 및 `AF_INET` 도메인 소켓 사용
  - `SOCK_STREAM` (TCP 기반 스트림 소켓) 활용
- **멀티스레딩**: `pthread`를 사용하여 멀티 클라이언트 지원
- **파일 전송**: 서버에서 클라이언트로 파일 전송 기능 구현
- **동기형 채팅**: 서버-클라이언트 간 1:1 동기화된 메시지 송수신

---

## 📜 주요 기능

### **1️⃣ AF_UNIX 도메인 소켓 프로그래밍**
- `af_unix_server.c` & `af_unix_client.c`
- **UNIX 도메인 소켓**을 사용하여 서버와 클라이언트 간 메시지 전송
- `connect()`를 통한 클라이언트 연결 및 `accept()`를 통한 서버 응답 처리
- `\quit` 입력 시 프로그램 종료

### **2️⃣ AF_INET 도메인 소켓 프로그래밍 (1:1 채팅)**
- `af_inet_server.c` & `af_inet_client.c`
- **TCP/IP 기반(AF_INET) 소켓**을 사용하여 서버-클라이언트 간 **동기형 1:1 채팅** 구현
- 클라이언트가 입력한 메시지를 서버에서 수신 후, 다시 서버의 메시지를 클라이언트로 전송
- `\quit` 입력 시 서버 및 클라이언트 모두 종료

### **3️⃣ 멀티서비스 서버**
- `multi_service_server.c` & `multi_service_client.c`
- 클라이언트는 서버에 접속 후 다음 **3가지 서비스 제공**
  1. **현재 시간 조회** (`get_localtime()`)
  2. **파일 다운로드** (텍스트/이미지 파일)
  3. **에코 서버** (입력한 메시지를 다시 클라이언트에게 반환)

---

## 🏗️ 빌드 및 실행 방법

### **1️⃣ 프로젝트 빌드**
```bash
make
```

## 📌 실행 예시
### **1️⃣ AF_UNIX 소켓 메시지 전송**
```bash
Client: Hello Server!
Server: Hello Client!
Client: \quit
```

### **2️⃣ AF_INET 동기형 채팅**
```bash
Client: 안녕하세요
Server: 반갑습니다!
Client: 오늘 날씨 좋네요
Server: 그러네요! 😊
Client: \quit
```

### **3️⃣ 멀티서비스 서버**
```bash
[Service List]
1. Get Current Time
2. Download File
3. Echo Server
Enter: 1
[Server Response] Current Time: 2025-02-01 12:30:45
```
```bash
Enter: 2
[Server Response] Available files:
1. example.txt
2. image.png
Enter file number to download: 1
[Server Response] Downloading example.txt...
File received successfully.
```
```bash
Enter: 3
[Server Response] Echo service started. Type a message:
Client: Hello, server!
[Server Response] Echo: Hello, server!
Client: How are you?
[Server Response] Echo: How are you?
Client: \quit
[Server Response] Echo service closed.
```
