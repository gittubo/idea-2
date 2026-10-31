/* Cindy Zhu guides — shared client kit.
   Adds a one-click "Copy" button to the top-right of every prompt block (.promptwrap),
   so viewers can copy a plan/build prompt straight into Claude, Replit, Emergent, etc.
   Load once per guide: <script src="../guide-kit.js" defer></script> */
(function () {
  function addCopyButtons() {
    var wraps = document.querySelectorAll(".promptwrap");
    wraps.forEach(function (wrap) {
      if (wrap.querySelector(".copybtn")) return;
      var code = wrap.querySelector("pre code") || wrap.querySelector("pre");
      if (!code) return;

      var btn = document.createElement("button");
      btn.type = "button";
      btn.className = "copybtn";
      btn.setAttribute("aria-label", "Copy prompt to clipboard");
      btn.textContent = "Copy";

      btn.addEventListener("click", function () {
        var text = code.innerText;
        var done = function () {
          btn.innerHTML = '<svg width="13" height="13" viewBox="0 0 24 24" fill="none" aria-hidden="true"><path d="M5 12.5l4.5 4.5L19 7.5" stroke="currentColor" stroke-width="2.6" stroke-linecap="round" stroke-linejoin="round"/></svg>Copied';
          btn.classList.add("copied");
          setTimeout(function () {
            btn.textContent = "Copy";
            btn.classList.remove("copied");
          }, 1600);
        };
        if (navigator.clipboard && navigator.clipboard.writeText) {
          navigator.clipboard.writeText(text).then(done).catch(fallback);
        } else {
          fallback();
        }
        function fallback() {
          try {
            var range = document.createRange();
            range.selectNodeContents(code);
            var sel = window.getSelection();
            sel.removeAllRanges();
            sel.addRange(range);
            document.execCommand("copy");
            sel.removeAllRanges();
            done();
          } catch (e) {
            btn.textContent = "Select + copy";
            setTimeout(function () { btn.textContent = "Copy"; }, 1600);
          }
        }
      });

      // one dark header row: prompt name on the left, Copy on the right (29 Oct 2026)
      var pre = wrap.querySelector("pre");
      if (pre && pre.parentNode === wrap) {
        var head = document.createElement("div");
        head.className = "prompthead";
        var label = wrap.querySelector(".promptlabel");
        if (!label) { label = document.createElement("span"); label.className = "promptlabel"; label.textContent = "prompt"; }
        head.appendChild(label);
        head.appendChild(btn);
        wrap.insertBefore(head, pre);
        wrap.classList.add("has-head");
      } else {
        wrap.appendChild(btn);
      }
    });
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", addCopyButtons);
  } else {
    addCopyButtons();
  }
})();

/* Shared entrance motion for every guide using this kit. */
(() => { if (!document.querySelector('script[data-site-reveals]')) {const script=document.createElement('script');script.src='/site-reveals.js';script.dataset.siteReveals='';script.defer=true;document.head.append(script);} })();
