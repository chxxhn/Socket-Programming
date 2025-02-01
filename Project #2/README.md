# 🖧 Socket Programming Project #2

## 📌 프로젝트 개요
이 프로젝트는 **컴퓨터 네트워크 소켓 프로그래밍** 실습을 위한 코드 구현입니다.  
**CTRL+C 시그널 핸들러, 멀티클라이언트 Echo 서버, select 기반 다중 채팅 서버**를 포함합니다.  
C 언어의 **socket API**를 활용하여 클라이언트-서버 간 통신을 직접 구현하였습니다.

---

## 🛠 사용 기술
- **프로그래밍 언어**: C
- **소켓 프로그래밍**:
  - `AF_INET`, `SOCK_STREAM` (TCP 기반 소켓)
  - `fork()`를 활용한 멀티클라이언트 서버
  - `select()`를 활용한 비동기 다중 채팅 서버
- **시그널 핸들링**:
  - `SIGINT` (CTRL+C) 시그널 처리
- **멀티클라이언트 서버**:
  - 클라이언트 접속 시 `client_cnt` 변수 증가
  - 서버에서 클라이언트의 메시지를 그대로 반환하는 Echo 기능

---

## 📜 주요 기능

### **1️⃣ CTRL+C 시그널 핸들러**
- `signal(SIGINT, handler)`를 이용하여 **CTRL+C 입력 시 종료 메시지 출력 후 종료**
- `sleep(1000)`을 사용하여 일정 시간 대기 후 종료

```c
void handler(int signum) {
    printf("Handler is called.\n");
    exit(EXIT_SUCCESS);
}
```

### **2️⃣ Concurrent server: Echo 서버**
- `fork()`를 이용하여 **클라이언트별로 별도의 프로세스를 생성**
- 클라이언트가 메시지를 보내면 **서버가 동일한 메시지를 반환**
- `client_cnt` 변수를 이용하여 접속된 클라이언트 수 출력

```c
printf("Number of service client : %d\n", client_cnt);
```

### **3️⃣ select 기반 MultiChat 서버**
- `select()` 함수를 이용하여 **여러 클라이언트의 요청을 비동기 처리**
- `FD_SET()` 및 `FD_ISSET()`을 활용하여 **소켓 이벤트 감지**
- **3명의 클라이언트가 동시에 접속 후 메시지를 주고받는 구조**

---

## 📌 실행 예시
### **1️⃣ CTRL+C 시그널 핸들러**
```bash
$ ./signal_handler
Sleep begins!
^C
Handler is called.
```

### **2️⃣ Concurrent server: Echo 서버**
```bash
Client: Hello Server!
Server: Recv from Server: Hello Server!
Client: \quit
```

### **3️⃣ select 기반 MultiChat 서버**
```bash
Client 1: 안녕하세요
Client 2: 반갑습니다!
Client 3: 채팅 잘 되네요~
```
