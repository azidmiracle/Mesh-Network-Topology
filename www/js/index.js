/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

// Wait for the deviceready event before using any of Cordova's device APIs.
// See https://cordova.apache.org/docs/en/latest/cordova/events/events.html#deviceready

let getSSIDbtn = document.getElementById("getSSIDbtn");
let connect = document.getElementById("connect");
let connecttoWebApp = document.getElementById("connecttoWebApp");
let lastResult = document.getElementById("lastResult");

getSSIDbtn.addEventListener("click", async function (e) {
  //alert(WifiWizard2.getConnectedSSID())
  let ssidName = "";
  let routerIP = "";
  ssidName = await WifiWizard2.getConnectedSSID();

  //get the regular expression for AP8266
  let checkSSID = /AP8266/.test(ssidName); //if the name matches, it will return true

  if (checkSSID) {
    routerIP = await WifiWizard2.getWifiRouterIP();
    // Here, we redirect to the web site.
    var targetUrl = "http://" + routerIP; //"http://10.250.98.1";
    var bkpLink = document.getElementById("bkpLink");
    bkpLink.setAttribute("href", targetUrl);
    bkpLink.text = targetUrl;
    window.location.replace(targetUrl);
  } else {
    lastResult.innerHTML = "Please connect to AP8266 Wifi";
  }
});
