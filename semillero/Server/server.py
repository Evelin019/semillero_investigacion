import socket
import time

# Configura la dirección IP y el puerto en el que el Arduino enviará los datos
# host = "0.0.0.0"  # Reemplaza con la dirección IP de tu Arduino

host = "172.29.223.6"  # Reemplaza con la dirección IP de tu Arduino
port = 80

# Crea un socket TCP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Enlaza el socket a la dirección IP y puerto
server_socket.bind((host, port))

# Escucha conexiones entrantes (1 a la vez)
server_socket.listen(1)

print(f"Esperando conexiones en {host}:{port}")

while True:
    # Acepta la conexión entrante
    client_socket, client_address = server_socket.accept()
    print(f"Conexión entrante desde {client_address}")

    while True:
        data = client_socket.recv(1024)  # Recibe datos del Arduino (ajusta el tamaño del búfer según sea necesario)
        if not data:
            break  # Si no hay datos, sal del bucle interno

        # Decodifica los datos y separa la temperatura y la humedad
        decoded_data = data.decode('utf-8')
        data_parts = decoded_data.split('&')
        temperature = None
        humidity = None

        for part in data_parts:
            if part.startswith("temperatura="):
                temperature = part.split('=')[1]
            elif part.startswith("humedad="):
                humidity = part.split('=')[1]

        if temperature is not None and humidity is not None:
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            print(f"Tiempo: {timestamp}, Temperatura: {temperature}°C, Humedad: {humidity}%")

    # Cierra la conexión con el cliente
    client_socket.close()

# Cierra el socket del servidor
server_socket.close()
