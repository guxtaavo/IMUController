using UnityEngine;
using System.IO.Ports;

public class SerialReader : MonoBehaviour
{
    public string portName = "COM9";  // Porta serial
    public int baudRate = 9600;       // Taxa de baud
    public GameObject cube;           // Objeto que ser� controlado

    private SerialPort serialPort;    // Objeto para comunica��o serial
    private string receivedData = ""; // Dados recebidos

    private Vector3 accelerometer = Vector3.zero; // Valores do aceler�metro

    public float rotationScale = 10.0f; // Fator de escala para a rota��o

    void Start()
    {
        try
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.Open();
            serialPort.DtrEnable = true;  // Garante a leitura cont�nua
            serialPort.ReadTimeout = 500;

            Debug.Log("Porta serial aberta com sucesso: " + portName);
        }
        catch (System.Exception e)
        {
            Debug.LogError("Erro ao abrir a porta serial: " + e.Message);
        }
    }

    void Update()
    {
        try
        {
            if (serialPort.IsOpen && serialPort.BytesToRead > 0)
            {
                receivedData = serialPort.ReadLine().Trim();

                if (!string.IsNullOrEmpty(receivedData))
                {
                    Debug.Log("Dados recebidos: " + receivedData);
                    ParseData(receivedData); // Processa os dados recebidos
                }
            }

            // Atualiza a rota��o do cubo com os dados do aceler�metro
            if (cube != null)
            {
                // Calcula a nova rota��o com os dados do aceler�metro
                Vector3 newRotation = new Vector3(
                    accelerometer.x * rotationScale,
                    accelerometer.y * rotationScale,
                    accelerometer.z * rotationScale
                );

                Debug.Log($"Nova rota��o calculada: {newRotation}");
                cube.transform.localRotation = Quaternion.Lerp(
                    cube.transform.localRotation,
                    Quaternion.Euler(newRotation),
                    Time.deltaTime
                );
            }
        }
        catch (System.Exception e)
        {
            Debug.LogError("Erro na leitura serial: " + e.Message);
        }
    }

    void ParseData(string data)
    {
        // Verifica o prefixo para determinar o tipo de dado
        if (data.StartsWith("Acelerometro:"))
        {
            string[] values = data.Replace("Acelerometro:", "").Split(',');
            if (values.Length == 3)
            {
                if (float.TryParse(values[0], out float ax)) accelerometer.x = ax;
                if (float.TryParse(values[1], out float ay)) accelerometer.y = ay;
                if (float.TryParse(values[2], out float az)) accelerometer.z = az;
            }
        }
    }

    void OnApplicationQuit()
    {
        if (serialPort.IsOpen)
        {
            serialPort.Close();
            Debug.Log("Porta serial fechada.");
        }
    }
}
