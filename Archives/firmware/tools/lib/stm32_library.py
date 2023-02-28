from codecs import encode

def stm32_crc32(data, crc):
    uint32_data = []
 #   for i in range(len(data)/4):
  #      uint32_data.append(0xFFFFFFFF & int(encode(data[(i*4)+3] + data[(i*4)+2] + data[(i*4)+1] + data[i*4], 'hex'), 16))
  
    for u_datum in uint32_data:
        crc = crc ^ u_datum
        for i in range(0, 32):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF  # Polynomial used in STM32
            else:
                crc = (crc << 1) & 0xFFFFFFFF
 
    return crc
