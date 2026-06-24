#include "Server_callbacks.h"

// The Handle function getting the message from the Web App
void handleMorse() {
  if (server.hasArg("message")) {
    //We Save the message and then we take the right to write using the mutex variable
    String msg = server.arg("message");
    if (xSemaphoreTake(mutex_message, portMAX_DELAY)){
      current_message = msg;
      new_message = 1;
      xSemaphoreGive(mutex_message);
    }
    Serial.print("Message reçu : ");
    Serial.println(msg);


    server.send(200, "text/plain", "OK : " + msg);
  } else {
    server.send(400, "text/plain", "Parametre manquant");
  }
}

void handleRoot() {
  String html = R"(
    <!doctype html>
  <html>
      <head>
          <meta charset="utf-8" />
          <title>Téléschtroumpf</title>
          <style>
              body {
                  background-color: #02b6ee;
                  margin: 0;
                  font-family: sans-serif;
              }

              .container {
                  display: flex;
                  flex-direction: column;
                  align-items: center;
                  padding: 2rem;
                  gap: 1.5rem;
              }

              h1 {
                  color: white;
                  margin: 0;
              }

              .form-row {
                  display: flex;
                  gap: 0.75rem;
                  align-items: center;
              }

              input[type="text"] {
                  padding: 0.5rem 1rem;
                  border: none;
                  border-radius: 6px;
                  font-size: 1rem;
              }

              button.envoyer {
                  padding: 0.5rem 1.2rem;
                  background: #fff;
                  color: #02b6ee;
                  border: none;
                  border-radius: 6px;
                  font-size: 1rem;
                  cursor: pointer;
                  transition: filter 0.1s;
              }

              button.envoyer:hover {
                  filter: brightness(0.92);
              }
              button.envoyer:active {
                  transform: scale(0.96);
              }

              button.svg-btn {
                  background: none;
                  border: none;
                  cursor: pointer;
                  padding: 0;
                  display: inline-block;
                  transition:
                      transform 0.1s,
                      filter 0.1s;
              }

              button.svg-btn:hover {
                  filter: brightness(1.2);
              }
              button.svg-btn:active {
                  transform: scale(0.95);
              }
          </style>
      </head>

      <body>
          <div class="container">
              <h1>Envoyer un message Morse</h1>

              <form action="/morse" method="POST">
                  <div class="form-row">
                      <input
                          type="text"
                          name="message"
                          placeholder="Texte à encoder"
                      />

                      <!-- Bouton SVG (même action, type submit) -->
                      <button type="submit" class="svg-btn">
                          <svg
                              width="80"
                              height="80"
                              viewBox="0 0 210 297"
                              xmlns="http://www.w3.org/2000/svg"
                          >
                              <path
                                  fill="#ff0000"
                                  d="m 52.915598,225.60577 c 0,0 -96.099519,-62.53281 -28.730783,-134.469049 22.118235,-23.617816 85.201635,-32.44183 85.201635,-32.44183 0,0 97.32156,-9.900239 76.2852,13.164811 C 156.94086,103.36119 33.101263,122.6382 201.02774,165.894 253.50626,179.41181 52.915598,225.60577 52.915598,225.60577 Z"
                              />
                              <path
                                  fill="#d43426"
                                  d="M 48.179947,221.90515 C 38.967992,214.985 28.163668,204.60356 21.37488,196.14924 -2.3001167,166.66593 -5.9528455,136.93368 10.696884,109.23317 14.557631,102.80996 20.142044,95.570666 25.149083,90.498207 37.053266,78.438482 63.20587,67.757277 97.191146,61.074897 106.85189,59.175348 110.5091,58.677481 119.58534,58.02632 c 36.9615,-2.651754 60.92911,-1.017489 67.05524,4.572252 2.85809,2.607842 2.27826,5.498023 -1.99368,9.937625 -5.188,5.3916 -13.43353,10.81903 -29.73608,19.573105 -22.96438,12.331308 -29.02868,16.069888 -34.38686,21.199138 -14.85247,14.21791 -1.88029,27.06614 42.0629,41.661 8.34042,2.7701 19.84958,6.16155 31.42696,9.26071 5.10469,1.36647 10.14698,2.83146 11.2051,3.25553 2.32448,0.9316 4.32315,2.81258 4.32315,4.06858 0,8.47851 -35.59028,22.41139 -103.52164,40.52665 -19.186327,5.11642 -51.54789,13.16789 -52.926184,13.16789 -0.254822,0 -2.46626,-1.50465 -4.914299,-3.34365 z"
                              />
                          </svg>
                      </button>
                  </div>
              </form>
          </div>
      </body>
  </html>

  )";
  server.send(200, "text/html", html);
}