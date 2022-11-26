# Protocol of Data 
## General frame
|B01 |B02 |B03 |B04 |B05 |<>  |B0n|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|no.B|no.B|head|Data|CRC|

> 0xAA + 0x55: Header of frame

> no.B: Number of bytes data (Include head + Data + CRC)

> head: Header of data packages

> Data: Data

> CRC: Checksum of Data 0xFF - (B03 + B04 + ... + B0n-1)

---
### List of header
|Header ID|Role|
|:---:|:---:|
|0x01|Start of OTA|
|0x02|Stop of OTA|
|0x03|Data OTA|
|0x04|Length of OTA|
|0x05|Checksum of OTA|
|0x06|Version of Firmware|
|0x07|Reboot|
---
### Example
**Start of OTA**
|B01 |B02 |B03 |B04 |B05 |B06|
|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x02|**0x01**|CRC|

**Stop of OTA**
|B01 |B02 |B03 |B04 |B05 |B06|
|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x02|**0x02**|CRC|

**Data OTA**
|B01 |B02 |B03 |B04 |B05 |B07|B08|B09|B10|<>|Bn1|Bn|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|<>|<>|**0x03**|Location of Data|Location of Data|Nbr Bytes|Nbr Bytes|Data<4>|Checksum|CRC|

**Length of OTA**
|B01 |B02 |B03 |B04 |B05 |B06|B07|B08|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x04|**0x04**|Length|Length|CRC|

**Checksum of OTA**
|B01 |B02 |B03 |B04 |B05 |B06|B07|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x03|**0x05**|Checksum|CRC|

**Version of OTA**
|B01 |B02 |B03 |B04 |B05 |B06|B07|B08|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x04|**0x06**|Major|Minor|CRC|

**Reboot**
|B01 |B02 |B03 |B04 |B05 |B06|
|:---:|:---:|:---:|:---:|:---:|:---:|
|0xAA|0x55|0x00|0x02|**0x07**|CRC|
