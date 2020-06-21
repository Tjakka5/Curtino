using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace App
{
    // Learn more about making custom code visible in the Xamarin.Forms previewer
    // by visiting https://aka.ms/xamarinforms-previewer
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {
        const string requestURL = "http://192.168.2.15/post/";

        public MainPage()
        {
            InitializeComponent();
        }

		private async void ButtonON_Clicked(object sender, EventArgs e)
		{
            HttpClient httpClient = new HttpClient();

			JObject data = new JObject
			{
				{ "command", "setLed" },

				{ "state", true }
			};

			//httpClient.PostAsync(requestURL, new HttpContent());
			HttpResponseMessage response = await httpClient.PostAsync(requestURL, new StringContent(data.ToString(), Encoding.UTF8, "application/json"));
			//HttpResponseMessage response = await httpClient.GetAsync(requestURL);
            if (response.IsSuccessStatusCode)
            {
                string content = await response.Content.ReadAsStringAsync();
                Console.WriteLine(content);
            }
        }

        private async void ButtonOFF_Clicked(object sender, EventArgs e)
        {
            HttpClient httpClient = new HttpClient();

            JObject data = new JObject
            {
                { "command", "setLed" },

                { "state", false }
            };

            //httpClient.PostAsync(requestURL, new HttpContent());
            HttpResponseMessage response = await httpClient.PostAsync(requestURL, new StringContent(data.ToString(), Encoding.UTF8, "application/json"));
            //HttpResponseMessage response = await httpClient.GetAsync(requestURL);
            if (response.IsSuccessStatusCode)
            {
                string content = await response.Content.ReadAsStringAsync();
                Console.WriteLine(content);
            }
        }

        private void OpenCurtainControls(object sender, EventArgs e)
        {
            var myValue = ((Button)sender).Text;

            Navigation.PushAsync(new ControlPage(myValue));
        }
    }
}
