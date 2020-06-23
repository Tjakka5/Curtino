using Newtonsoft.Json.Linq;
using System;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace App
{
	class CurtainCommunication
	{
		const string requestURL = "http://84.82.92.7/sendCommand/";

		public async Task<string> SendMessage(JObject data)
		{
			HttpClient httpClient = new HttpClient();

			StringContent stringContent = new StringContent(data.ToString(), Encoding.UTF8, "application/json");
			HttpResponseMessage response = await httpClient.PostAsync(requestURL, stringContent);

			if (response.IsSuccessStatusCode)
			{
				return await response.Content.ReadAsStringAsync();
			}
			else 
				return string.Empty;
		}
	}
}
