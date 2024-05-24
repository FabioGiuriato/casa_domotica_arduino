using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort;

        public Form1()
        {
            InitializeComponent();
            serialPort = new SerialPort();
            serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            InitializeDataGridView();
        }

        private void InitializeDataGridView()
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort.PortName = "COM3";  // Sostituisci con la tua porta COM
                serialPort.BaudRate = 9600;
                serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            string data = serialPort.ReadLine();
            string datastamp = DateTime.Now.ToString("yyyy-MM-dd");
            string orestamp = DateTime.Now.ToString("HH:mm:ss.fff");

            // Supponiamo che i dati siano nel formato "Temperatura:XX Umidità:YY"
            string[] parts = data.Split(' '); // Divide i dati per spazi
            string temperatura = parts[0].Split(':')[1];
            string umidità = parts[1].Split(':')[1];

            Invoke(new MethodInvoker(delegate
            {
                tabella.Rows.Add(new string[] { datastamp, orestamp, temperatura, umidità });
            }));
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }
    }
}
