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
});
