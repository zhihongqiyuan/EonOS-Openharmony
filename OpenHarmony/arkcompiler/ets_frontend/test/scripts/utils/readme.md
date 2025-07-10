# Instructions for using the utils package

## Script Purpose
* Download the necessary image files for testing
* Obtain the burning tool and burn the downloaded Dayu image
* Retrieve the repository commit information from 00:00 on the day before the script run to the time the script is executed
* Automatically send test results via email

## Dependency Installation
```
python3 -m pip install pywinauto lxml requests
```

## Notes
* The storage path for the SDK needs to be strictly filled in according to the structure of the SDK path in the new version of IDEA
* If no content is provided for sdkUrl、dayuUrl, the script will automatically retrieve the image address from the official website
* Executing without providing parameters will follow the configured settings