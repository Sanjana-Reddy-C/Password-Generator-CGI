let history = [];

// Slider live update
const lengthSlider = document.getElementById("length");
const lengthLabel = document.getElementById("lengthLabel");

lengthLabel.innerText = "Password Length: " + lengthSlider.value;

lengthSlider.addEventListener("input", () => {
  lengthLabel.innerText = "Password Length: " + lengthSlider.value;
});

// Main generate function
function generate() {
  const lower   = document.getElementById("lower").checked;
  const upper   = document.getElementById("upper").checked;
  const digits  = document.getElementById("digits").checked;
  const special = document.getElementById("special").checked;

  const strengthChoice = document.getElementById("strengthSelect").value;
  const length = lengthSlider.value;

  // Validation: at least one checkbox
  if (!lower && !upper && !digits && !special) {
    alert("Select at least one character type.");
    return;
  }

  // Validation for strong passwords
  if (strengthChoice === "strong") {
    const typesCount = lower + upper + digits + special;

    if (length < 12) {
      alert("Strong passwords must be at least 12 characters long.");
      return;
    }

    if (typesCount < 3) {
      alert("Strong passwords require at least 3 character types.");
      return;
    }
  }

  const url =
    `/cgi-bin/password_generator.cgi?length=${length}` +
    `&lower=${+lower}&upper=${+upper}&digits=${+digits}&special=${+special}`;

  fetch(url)
    .then(res => res.text())
    .then(data => {

      // If Apache returned HTML (wrong server)
      if (data.includes("<html")) {
        alert("Server error: CGI returned HTML. Use Apache (localhost).");
        console.error(data);
        return;
      }

      const lines = data.trim().split("\n");

      // Backend error
      if (lines[0] === "ERROR") {
        alert(lines[1]);
        return;
      }

      // Success
      const password = lines[0].split(": ")[1];
      const strength = lines[1];

      document.getElementById("password").innerText = password;
      document.getElementById("strength").innerText = strength;

      addToHistory(password);
    });
}

// History
function addToHistory(password) {
  const historyDiv = document.getElementById("history");
  const item = document.createElement("div");
  item.innerText = password;
  historyDiv.prepend(item);

  history.push(password);
}
