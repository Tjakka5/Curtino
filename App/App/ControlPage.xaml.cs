using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace App
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class ControlPage : ContentPage
	{
		CurtainCommunication CurtainControls;
		string curtino = "";

		public ControlPage(string curtino)
		{
			InitializeComponent();
			this.curtino = curtino;		
			Title = curtino;			// Set the page title to the name of the curtino.

			CurtainControls = new CurtainCommunication();
		}

		private async void Submit_Data(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "configureLight" },
				{ "lightRequiredToOpen", Int32.Parse(LightOpenValue.Text) },
				{ "lightRequiredToClose", Int32.Parse(LightCloseValue.Text) }
			};
			await CurtainControls.SendMessage(data);
		}

		private async void Open_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "openCurtain" }
			};
			var responseTask = CurtainControls.SendMessage(data);
			await responseTask;

			JObject response = JObject.Parse(responseTask.Result);
			CurtainStateText.Text = response["state"].ToString();
		}

		private async void Close_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "closeCurtain" }
			};
			var responseTask = CurtainControls.SendMessage(data);
			await responseTask;

			JObject response = JObject.Parse(responseTask.Result);
			CurtainStateText.Text = response["state"].ToString();
		}

		private async void Stop_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "stopCurtain" }
			};
			var responseTask = CurtainControls.SendMessage(data);
			await responseTask;

			JObject response = JObject.Parse(responseTask.Result);
			CurtainStateText.Text = response["state"].ToString();
		}
	}
}