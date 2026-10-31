/* Topic ribbon: fills the strip with enough copies to loop seamlessly.
   Same behaviour as the inline script on the current site, moved into one file. */
(() => {
  const motion = window.matchMedia('(prefers-reduced-motion: reduce)');
  document.querySelectorAll('.growth-ribbon').forEach(ribbon => {
    const track = ribbon.querySelector('.growth-ribbon-track');
    const first = ribbon.querySelector('.growth-ribbon-copy');
    function sync() {
      ribbon.classList.toggle('is-moving', !motion.matches);
      if (motion.matches) return;
      const width = first.getBoundingClientRect().width;
      if (!width) return;
      const count = Math.max(2, Math.ceil(ribbon.clientWidth / width) + 1);
      while (track.children.length < count) {
        const copy = first.cloneNode(true);
        copy.classList.add('growth-ribbon-duplicate');
        track.append(copy);
      }
      while (track.children.length > count) track.lastElementChild.remove();
      ribbon.style.setProperty('--ribbon-step', `${width}px`);
      ribbon.style.setProperty('--ribbon-duration', `${width / 36}s`);
    }
    sync();
    document.fonts.ready.then(sync);
    window.addEventListener('resize', sync);
    motion.addEventListener('change', sync);
  });
})();
