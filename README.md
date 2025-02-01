# 🖧 Socket-Programming

## 📌 개요
이 레포지토리는 **컴퓨터 네트워크 소켓 프로그래밍 실습 프로젝트**들을 포함합니다.  
C 언어를 활용하여 **AF_UNIX 및 AF_INET 소켓, 멀티클라이언트 서버, select 기반 다중 채팅 서버** 등을 구현하였습니다.  
또한, **시그널 핸들링 및 파일 전송 기능**도 포함되어 있으며, 다양한 네트워크 통신 기법을 실습합니다.

---

## 📜 주요 기능

### **1️⃣ 기본 소켓 프로그래밍**
- **기능:** AF_UNIX 및 AF_INET 소켓을 활용한 1:1 통신 및 멀티서비스 서버 구현
- **사용 기술:** `AF_UNIX`, `AF_INET`, `SOCK_STREAM`, `fork()`, `pthread`
- **주요 파일:**
  - `af_unix_server.c`, `af_unix_client.c` → UNIX 도메인 소켓 메시지 전송
  - `af_inet_server.c`, `af_inet_client.c` → TCP 기반 1:1 채팅
  - `multi_service_server.c`, `multi_service_client.c` → 여러 기능을 제공하는 멀티서비스 서버

### **2️⃣ CTRL+C 시그널 핸들러 및 Echo 서버**
- **기능:** `SIGINT`(CTRL+C) 시그널 처리 및 멀티클라이언트 Echo 서버
- **사용 기술:** `signal()`, `fork()`, `SOCK_STREAM`
- **주요 파일:**
  - `signal_handler.c` → CTRL+C 입력 시 종료 메시지 출력 후 종료
  - `echo_server.c`, `echo_client.c` → 멀티클라이언트 Echo 서버

### **3️⃣ select 기반 다중 채팅 서버**
- **기능:** `select()`를 활용하여 여러 클라이언트와 비동기 채팅 지원
- **사용 기술:** `FD_SET()`, `FD_ISSET()`, `select()`
- **주요 파일:**
  - `select_chat_server.c`, `select_chat_client.c` → select 기반 다중 채팅 서버
 
---

## 🛠 사용 기술
- **프로그래밍 언어:** C
- **소켓 프로그래밍:** `AF_UNIX`, `AF_INET`, `SOCK_STREAM`
- **멀티클라이언트 지원:** `fork()`, `pthread`
- **비동기 이벤트 처리:** `select()`
- **시그널 핸들링:** `SIGINT`, `signal()`
- **파일 전송:** `send()`, `recv()`
- **채팅 서버:** 다중 사용자 지원, 클라이언트 간 메시지 공유
