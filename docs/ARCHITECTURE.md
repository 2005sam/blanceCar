# ARCHITECTURE

## document information

- data:2026/3/10
- author : sam

## Overall Architecture Overview

```mermaid
graph TD
	subgraph application
	ControlSystemTask
	Mpu6050Task
	AttitudeTask
	EncoderTask
	BluetoothTask
	end
	subgraph OSAL
	end
	subgraph BSP
		UART/bluetooth
		motor
		mpu6050
		bsp_led
		button
	end
	subgraph Component
	end
	subgraph HAL
	end
	application <--> Component
	application <--> BSP
	application <-->OSAL
	BSP <--> HAL
```



