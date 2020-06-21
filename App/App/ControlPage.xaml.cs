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
		string curtain = "";

		public ControlPage(string curtain)
		{
			InitializeComponent();
			this.curtain = curtain;
			Title = curtain;
		}

		private void Submit_Data(object sender, EventArgs e)
		{

		}

		private void Open_Curtain(object sender, EventArgs e)
		{

		}

		private void Close_Curtain(object sender, EventArgs e)
		{

		}

		private void Stop_Curtain(object sender, EventArgs e)
		{

		}
	}
}