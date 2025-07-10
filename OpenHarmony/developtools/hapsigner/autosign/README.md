# Instructions
Autosign help you to sign app more efficiently


## Dependency
This signature script depends on Python 3.x.



## Configuration

Use your favorite text editor to open `autosign.config` to configure everything.
Below config must be replaced:
* config.signtool
* All password

### 1. For Windows

1. Run the `create_root.bat` file in the terminal to generate root certs.
2. Prepare your unsigned app and provision profile. Make sure correct config file location.
3. Run the `create_appcert_sign_profile.bat` file in the terminal to generate app certs.
4. Run the `sign_hap.bat` file in the terminal to start signing.
5. The generated artifacts will be saved in the folder you set in `config.targetDir` as default.

### 2. For Linux or MacOS


1. Add executable permission into `create_root.sh`, `create_appcert_sign_profile.sh` and `sign_hap.sh`.

   ```bash
   chmod a+x create_root.sh
   
   chmod a+x create_appcert_sign_profile.sh

   chmod a+x sign_hap.sh

   ```

2. Run the `create_root.sh` file in the terminal to generate root certs.

   ```bash
   ./create_root.sh
   ```
3. Prepare your unsigned app and provision profile. Make sure correct config file location.
4. Run the `create_appcert_sign_profile.sh` file in the terminal to generate app certs.
    ```bash
    ./create_appcert_sign_profile.sh
    ```
5. Run the `sign_hap.sh` file in the terminal to start signing.
   ```bash
   ./sign_hap.sh
   ```
6. The generated artifacts will be saved in the folder you set in `config.targetDir` as default.
