using Newtonsoft.Json.Linq;
using System;
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
			Title = curtino;            // Set the page title to the name of the curtino.

			CurtainControls = new CurtainCommunication();

			Device.StartTimer(TimeSpan.FromSeconds(1), () => { UpdateState(); return true; });
		}

		private async void Submit_Data(object sender, EventArgs e)
		{
			if (!Int32.TryParse(LightOpenValue.Text, out int lightRequiredToOpen))
				lightRequiredToOpen = 0;
			if (!Int32.TryParse(LightOpenValue.Text, out int lightRequiredToClose))
				lightRequiredToClose = 0;

			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "configureLight" },
				{ "lightRequiredToOpen", lightRequiredToOpen },
				{ "lightRequiredToClose", lightRequiredToClose }
			};
			await CurtainControls.SendMessage(data);
		}

		private async void UpdateState()
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "getCurtainStatus" }
			};

			var responseTask = CurtainControls.SendMessage(data);
			await responseTask;

			JObject response = JObject.Parse(responseTask.Result);
			if ((bool)response["success"])
			{
				CurtainStateText.Text = response["status"].ToString();
				CurrentLightValue.Text = response["lightStatus"].ToString();
			}
		}

		private async void Open_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "openCurtain" }
			};
			await CurtainControls.SendMessage(data);
		}

		private async void Close_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "closeCurtain" }
			};
			await CurtainControls.SendMessage(data);
		}

		private async void Stop_Curtino(object sender, EventArgs e)
		{
			JObject data = new JObject
			{
				{ "deviceID", curtino },
				{ "command", "stopCurtain" }
			};
			await CurtainControls.SendMessage(data);
		}

	}
}