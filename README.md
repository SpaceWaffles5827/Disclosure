# Disclosure
# Remote Monitoring and Control Project

## Overview

This project implements a remote monitoring and control system that includes keylogging, webcam access, and remote desktop control. The primary purpose of this project is for educational purposes, to understand how remote monitoring and control systems work, and for legitimate security testing in controlled environments with explicit consent.

**IMPORTANT: This software is intended solely for ethical and legal purposes. Unauthorized use of remote monitoring tools is illegal and unethical. Do not use this software for malicious purposes. Always obtain explicit consent before running this software in any environment.**

## Setup

1. **Clone the Repository:**
   ```sh
   git clone https://github.com/YourUsername/YourRepository.git
   cd YourRepository
   ```

2. **Create the `config.h` File:**
   - Create a file named `config.h` in the project directory with the following content:
     ```cpp
     #ifndef CONFIG_H
     #define CONFIG_H

     #define IP_ADDRESS "YOUR_IP_ADDRESS"  // Replace with your actual IP address

     #endif
     ```

   - Ensure `config.h` is added to your `.gitignore` to prevent it from being committed to version control:
     ```sh
     echo "config.h" >> .gitignore
     ```

3. **Compile the Project:**
   - Open the project in Visual Studio.
   - Build the solution to create the executable.

4. **Run the Executable:**
   - Execute the program in a controlled environment where you have explicit permission for remote monitoring and control.

## Features

- **Key Logging:** Captures and logs key presses.
- **Webcam Access:** Captures images or streams video from the webcam.
- **Remote Desktop Control:** Allows for remote control of the desktop environment.
- **TCP Transmission:** Sends logged data and remote control commands to a specified IP address over a TCP connection.
- **Console Hiding:** Hides the console window to run in the background.
- **Graceful Termination:** Handles SIGINT signal for graceful termination.

## Important Notes

- **Ethical Use:** Use this software only for ethical and legal purposes. Unauthorized remote monitoring is illegal and unethical.
- **Explicit Consent:** Always obtain explicit consent before running this software in any environment.
- **Security:** Ensure that the IP address and any sensitive information are securely managed and protected.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is provided "as is" without any warranties or guarantees. The authors are not responsible for any misuse of this software. Use it at your own risk and responsibility.

---

**Example `config.h` File:**

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define IP_ADDRESS "YOUR_IP_ADDRESS"  // Replace with your actual IP address

#endif
```

Replace `YOUR_IP_ADDRESS` with the actual IP address where you want to send the logged data and control commands.

---

For any issues or contributions, please open an issue or a pull request on GitHub.
