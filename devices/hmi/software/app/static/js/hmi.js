class PinLogin {
    constructor ({el, loginEndpoint, redirectTo, maxNumbers = Infinity}) {
        this.el = {
            main: el,
            numPad: el.querySelector(".pin-login__numpad"),
            textDisplay: el.querySelector(".pin-login__text")
        };

        this.maxNumbers = maxNumbers;
        this.value = "";

        this._generatePad();                      // underscore ist für private class
    }

    _generatePad() {
        const padLayout = [
            "1", "2", "3",
            "4", "5", "6",
            "7", "8", "9",
            "backspace", "0", "done"                                  //layout hier durch js ersetzt
        ];

        padLayout.forEach(key => {
            const insertBreak = key.search(/[369]/) !== -1;
            const keyEl = document.createElement("div");

            keyEl.classList.add("pin-login__key");
            keyEl.classList.toggle("material-icons", isNaN(key));
            keyEl.textContent = key;
            keyEl.addEventListener("click", () => { this._handleKeyPress(key) });
            this.el.numPad.appendChild(keyEl);

            if (insertBreak) {
                this.el.numPad.appendChild(document.createElement("br"));
            }
        });
    }

    _handleKeyPress(key) {
        switch (key) {
            case "backspace":
                this.value = this.value.substring(0, this.value.length - 1);        //basic delete
                break;
            case "done":
                this._attemptLogin();
                break;
            default:
                if (this.value.length < this.maxNumbers && !isNaN(key)) {
                    this.value += key;
                }
                break;
        }

        this._updateValueText();
    }

    _updateValueText() {
        this.el.textDisplay.value = "_".repeat(this.value.length);                // Schutz gegen chrome dev exploit document.querySelector(".pin-login__text").value in console
        this.el.textDisplay.classList.remove("pin-login__text--error");
    }

    _attemptLogin() {
        if (this.value.length > 0) {
            this.el.textDisplay.value = this.value;
            document.getElementById("myForm").submit(); 
        }
    }
}
