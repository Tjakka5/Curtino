import express from "express";
import { NextFunction, Request, Response } from "express";
var ping = require("ping");
var axios = require("axios");

const app = express();
const port = 80;

type DeviceIP = string;
type DeviceID = string;

const registeredDevices: Map<DeviceID, DeviceIP> = new Map();

const respond = (res: Response, statusCode: number, data: any = {}): void => {
  data.success = statusCode == 200;

  res.status(statusCode);
  res.json(data);
};

const errorHandler = async (
  _err: any,
  _req: Request,
  res: Response,
  _next: NextFunction
) => {
  respond(res, 500);
};

const cleanupDevices = (): void => {
  registeredDevices.forEach((deviceIP, deviceID) => {
    ping.sys.probe(deviceIP, (isAlive: boolean) => {
      if (!isAlive) {
        console.log(`${deviceIP} dead`);
        registeredDevices.delete(deviceID);
      }
    });
  });
};

app.use(express.json());
app.use(errorHandler);

app.post("/ping", (_req, res) => {
  respond(res, 200, {
    "value": "pong",
  });
})

app.post("/registerDevice", (req, res) => {
  let deviceID: DeviceID | undefined = req.body["deviceID"];
  let deviceIP: DeviceIP | undefined = req.body["deviceIP"];

  if (deviceID == undefined || deviceIP == undefined) {
    respond(res, 400);
  } else {
    registeredDevices.set(deviceID, deviceIP);
    console.log(`${deviceIP} registered under ${deviceID}`);

    respond(res, 200);
  }
});

app.post("/sendCommand", (req, res) => {
  let id: DeviceID | undefined = req.body['deviceID'];
  if (id == undefined) {
    respond(res, 400);
  } else {
    if (registeredDevices.has(id)) {
      let ip: DeviceIP = registeredDevices.get(id);
      console.log(`redirecting to ${id}`);
      axios.post(`http://${ip}/post`, req.body)
        .then(function(_response: any) {
          respond(res, 200)
        })
        .catch((err: any) => {
          console.log(err);
        })
    } else {
      console.log(`id ${id} not found`);
      respond(res, 400)
    }
  }
})

// app.post("/getDeviceIP", (req, res) => {
//   let deviceID: DeviceID | undefined = req.body["deviceID"];

//   if (deviceID == undefined) {
//     respond(res, 400);
//   } else {
//     let deviceIP: DeviceIP | undefined = registeredDevices.get(deviceID);

//     if (deviceIP == undefined) {
//       respond(res, 400);
//     } else {
//       respond(res, 200, {
//         deviceIP: deviceIP,
//       });
//     }
//   }
// });

setInterval(cleanupDevices, 1000 * 60 * 10); // 10 minutes
//setInterval(cleanupDevices, 1000 * 5); // 5 second

app.listen(port, () => {
  console.log(`server started at http://localhost:${port}`);
});
