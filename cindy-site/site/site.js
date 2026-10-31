/* Cindy Zhu site: shared behaviour on every page (29 Oct 2026).
   1. Waitlist forms post to Netlify in the background and turn into "you're on the list".
   2. Guide pages on wide screens get the "p.s. the waitlist is now open" side card. */
(function () {
  var CHECK = '<svg viewBox="0 0 24 24" fill="none" aria-hidden="true"><path d="M5 12.5l4.5 4.5L19 7.5" stroke="currentColor" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round"/></svg>';

  function wire(form) {
    if (form.dataset.wired) return;
    form.dataset.wired = "1";
    form.addEventListener("submit", function (e) {
      if (!window.fetch || !window.URLSearchParams) return; // old browsers do a normal post
      e.preventDefault();
      var btn = form.querySelector("button[type=submit]");
      if (btn) btn.disabled = true;
      var old = form.querySelector(".wl-err");
      if (old) old.remove();
      var data = new URLSearchParams(new FormData(form));
      if (!data.get("form-name")) data.set("form-name", form.getAttribute("name") || "waitlist");
      fetch("/", { method: "POST", headers: { "Content-Type": "application/x-www-form-urlencoded" }, body: data.toString() })
        .then(function (r) {
          if (!r.ok) throw new Error(r.status);
          var done = document.createElement("div");
          done.className = "wl-done";
          done.setAttribute("role", "status");
          done.innerHTML = '<span class="yes">' + CHECK + "you're on the list ✨</span>"; // Cindy 29 Oct: no second "first dibs" line
          form.replaceWith(done);
        })
        .catch(function () {
          if (btn) btn.disabled = false;
          var err = document.createElement("p");
          err.className = "wl-err";
          err.textContent = "That didn't go through. Check your email and try again.";
          form.appendChild(err);
        });
    });
  }

  function psCard() {
    if (!document.querySelector(".guidebody") || document.body.innerHTML.indexOf("sponsored-guide:") > -1) return;
    var card = document.createElement("aside");
    card.className = "ps-card";
    card.setAttribute("aria-label", "AI System Club waitlist");
    card.innerHTML =
      '<span class="eyebrow">p.s. the waitlist is now open</span><h3>AI System Club</h3>' +
      "<p>If you're keen on using AI to automate your content, video editing, job search, business or build AI agents, feel free to sign up (only if you want to hehe). Ps. the waitlist gets first dibs:)</p>" +
      '<form class="wl-form" name="waitlist" method="POST" action="/"><input type="hidden" name="form-name" value="waitlist">' +
      '<label class="sr-only" for="ps-email">Your email</label><input id="ps-email" type="email" name="email" placeholder="your email" autocomplete="email" required>' +
      '<button type="submit" class="btn">Join the waitlist</button></form>';
    document.body.appendChild(card);
    document.body.classList.add("has-ps");
    // hidden while the "btw the waitlist" line under the intro is on screen (no double note),
    // and once the waitlist card at the end comes into view
    var top = document.querySelector(".gcallout.orange");
    var end = document.querySelector(".wl-card") || document.querySelector(".guide-end");
    var topOn = !!top, endOn = false;
    function sync() { card.classList.toggle("away", topOn || endOn); }
    sync();
    if ("IntersectionObserver" in window) {
      if (top) new IntersectionObserver(function (es) {
        es.forEach(function (en) { topOn = en.isIntersecting || en.boundingClientRect.top > window.innerHeight; sync(); });
      }).observe(top);
      if (end) new IntersectionObserver(function (es) {
        es.forEach(function (en) { endOn = en.isIntersecting || en.boundingClientRect.top < 0; sync(); });
      }).observe(end);
    } else { topOn = false; sync(); }
  }

  function init() {
    psCard();
    document.querySelectorAll('form[name="waitlist"]').forEach(wire);
  }
  if (document.readyState === "loading") document.addEventListener("DOMContentLoaded", init);
  else init();
})();
