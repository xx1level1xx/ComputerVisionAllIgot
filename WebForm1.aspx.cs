using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.IO;
using System.Drawing;
using System.Text;
using Emgu.CV;
using Emgu.CV.Structure;
using System.Drawing.Imaging;
using System.Web;
using Twilio;
using Twilio.Rest.Api.V2010.Account;
using Twilio.Types;
using System.Threading;
namespace Camera_Final_4
{
    public partial class WebForm1 : System.Web.UI.Page
    {
        #region Parallel_Loop
        /*static void ParallelTest()
        {
            SqlConnection con = new SqlConnection();
            con.ConnectionString = "workstation id=ImageImage.mssql.sample.com;packet size=4096;user id=xx2level2xx_Sample;pwd=samplePass;data source=ImageImage.mssql.sample.com;persist security info=False;initial catalog=ImageImage;";
            con.Open();
            SqlCommand cmd;
            SqlDataReader rd;
            string tempblue = "";
            string tempgreen = "";
            string tempred = "";
            string blue;
            string green;
            string red;
            Image<Bgr, Byte> original = new Image<Bgr, byte>(640, 480);
            Bgr color = new Bgr();
            int countBlue = 0;
            int countGreen = 0;
            int countRed = 0;
            int count2 = 0;
            for(int i=0;i<640;i++)
            {
                cmd = new SqlCommand();
                cmd.CommandText = "SELECT ImageStringBlue, ImageStringGreen, ImageStringRed FROM test2 WHERE ID='" + i + "'";
                cmd.Connection = con;
                rd = cmd.ExecuteReader();
                rd.Read();
                blue = rd[0].ToString();
                green = rd[1].ToString();
                red = rd[2].ToString();
                
                count2 = 0;
                countBlue = 0;
                countGreen = 0;
                countRed = 0;
                while (count2 < 480)
                {
                    Parallel.For(0, 3, j =>
                    {
                        if (j == 0)
                        {
                            tempblue = "";
                            for (int k = countBlue; k < blue.Length && blue[k] != ' '; k++)
                            {
                                tempblue += blue[k];
                                if (k + 1 == blue.Length || blue[k + 1] == ' ')
                                {
                                    countBlue = k + 2;
                                }
                            }
                            color.Blue = convert(tempblue);
                        }
                        else if (j == 1)
                        {
                            tempgreen = "";
                            for (int k = countGreen; k < green.Length && green[k] != ' '; k++)
                            {
                                tempgreen += green[k];
                                if (k + 1 == green.Length || green[k + 1] == ' ')
                                {
                                    countGreen = k + 2;
                                }
                            }
                            color.Green = convert(tempgreen);
                        }
                        else
                        {
                            tempred = "";
                            for (int k = countRed; k < red.Length && red[k] != ' '; k++)
                            {
                                tempred += red[k];
                                if (k + 1 == red.Length || red[k + 1] == ' ')
                                {
                                    countRed = k + 2;
                                }
                            }
                            color.Red = convert(tempred);
                        }
                    }); // Parallel.For
                    original[count2, i] = color;
                    count2++;
                }
                rd.Close();
            }
            Mat final = original.Mat;
            Bitmap test = new Bitmap(final.Bitmap);
            test.Save(Server.MapPath("~/Images/" + "test60.png"), System.Drawing.Imaging.ImageFormat.Png);
            con.Close();
        }*/
        #endregion
        protected void Page_Load(object sender, EventArgs e)
        {
            //ParallelTest();
            SqlConnection con = new SqlConnection();
            con.ConnectionString = "workstation id=ImageImage.mssql.sample.com;packet size=4096;user id=xx2level2xx_Sample;pwd=samplePass;data source=ImageImage.mssql.sample.com;persist security info=False;initial catalog=ImageImage;";
            con.Open();
            SqlCommand cmd;
            SqlDataReader rd;
            string tempblue = "";
            string tempgreen = "";
            string tempred = "";
            string blue;
            string green;
            string red;
            Image<Bgr, Byte> original = new Image<Bgr, byte>(640, 480);
            Bgr color = new Bgr();
            int countBlue = 0;
            int countGreen = 0;
            int countRed = 0;
            int count2 = 0;
            for (int i = 0; i < 640; i++)
            {
                cmd = new SqlCommand();
                cmd.CommandText = "SELECT ImageStringBlue, ImageStringGreen, ImageStringRed FROM test2 WHERE ID='" + i + "'";
                cmd.Connection = con;
                rd = cmd.ExecuteReader();
                rd.Read();
                blue = rd[0].ToString();
                green = rd[1].ToString();
                red = rd[2].ToString();

                count2 = 0;
                countBlue = 0;
                countGreen = 0;
                countRed = 0;
                while (count2 < 480)
                {
                    Parallel.For(0, 3, j =>
                    {
                        if (j == 0)
                        {
                            tempblue = "";
                            for (int k = countBlue; k < blue.Length && blue[k] != ' '; k++)
                            {
                                tempblue += blue[k];
                                if (k + 1 == blue.Length || blue[k + 1] == ' ')
                                {
                                    countBlue = k + 2;
                                }
                            }
                            color.Blue = convert(tempblue);
                        }
                        else if (j == 1)
                        {
                            tempgreen = "";
                            for (int k = countGreen; k < green.Length && green[k] != ' '; k++)
                            {
                                tempgreen += green[k];
                                if (k + 1 == green.Length || green[k + 1] == ' ')
                                {
                                    countGreen = k + 2;
                                }
                            }
                            color.Green = convert(tempgreen);
                        }
                        else
                        {
                            tempred = "";
                            for (int k = countRed; k < red.Length && red[k] != ' '; k++)
                            {
                                tempred += red[k];
                                if (k + 1 == red.Length || red[k + 1] == ' ')
                                {
                                    countRed = k + 2;
                                }
                            }
                            color.Red = convert(tempred);
                        }
                    }); // Parallel.For
                    original[count2, i] = color;
                    count2++;
                }
                rd.Close();
            }
            Mat final = original.Mat;
            Bitmap test = new Bitmap(final.Bitmap);
            test.Save(Server.MapPath("~/Images/" + "test60.png"), System.Drawing.Imaging.ImageFormat.Png);

            var accountSid = "Sample";
            var authToken = "27284f81b9db175192842b596ab70100";
            TwilioClient.Init(accountSid, authToken);
            var mediaUrl = new[] {
            new Uri("http://www.sample4321.sample.com/Images/test60.jpg")
            }.ToList();
            var message = MessageResource.Create(
                 body: "Detected at "+ DateTime.Now.TimeOfDay.ToString(),
                 mediaUrl: mediaUrl,
                 from: new Twilio.Types.PhoneNumber("+14153476915"),
                 to: new Twilio.Types.PhoneNumber("+1626sampleNOW")
             );
            Console.WriteLine(message.Body);
            con.Close();
        }
        static private int convert(string num)
        {
            int sum = 0;
            for (int i = 0; i < num.Length; i++)
            {
                sum += power10(num.Length - i - 1) * (num[i] - '0');
            }
            return sum;
        }
        static private int power10(int num)
        {
            int prod = 1;
            if (num == 0)
            {
                return 1;
            }
            else
            {
                for (int i = 1; i <= num; i++)
                {
                    prod *= 10;
                }
            }
            return prod;
        }
    }
}