document.addEventListener("DOMContentLoaded", () => {
  const navLinks = document.querySelectorAll('.site-nav a[href^="#"]');

  navLinks.forEach((link) => {
    link.addEventListener("click", (event) => {
      const href = link.getAttribute("href");

      event.preventDefault();

      const targetId = href.slice(1);
      const targetSection = document.getElementById(targetId);

      targetSection.scrollIntoView({
        behavior: "smooth",
        block: "start",
      });
    });
  });

  const contactForm = document.querySelector(".contact-form");

  if (contactForm) {
    const messageField = contactForm.querySelector("#message");

    const counterEl = document.createElement("p");
    counterEl.className = "form-help";
    counterEl.textContent = "0 / 500 characters";
    messageField.insertAdjacentElement("afterend", counterEl);

    const updateCounter = () => {
      const length = messageField.value.length;
      counterEl.textContent = `${length} / 500 characters`;
    };

    updateCounter();

    messageField.addEventListener("input", updateCounter);

    contactForm.addEventListener("submit", (event) => {
      if (messageField.value.trim().length < 10) {
        event.preventDefault();
        counterEl.textContent =
          "Message should be at least 10 characters long.";
      }
    });
  }
});
