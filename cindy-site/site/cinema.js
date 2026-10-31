/* Cinematic layer for the main pages: hero entrance, pasted-in reveals,
   self-drawing marks, parallax depth, scroll-linked scenes and the film reel.
   Progressive: with no JS or with reduced motion, every page is complete and still. */
(() => {
  const reduce = matchMedia('(prefers-reduced-motion: reduce)');
  if (reduce.matches || !('IntersectionObserver' in window)) return;
  document.documentElement.classList.add('cine');

  // Pasted in as they arrive; siblings stagger. Marks draw themselves.
  // A fully clipped element never counts as visible, so [data-print] is watched through its parent.
  const io = new IntersectionObserver(entries => entries.forEach(e => {
    if (!e.isIntersecting) return;
    const el = e.target.printChild || e.target;
    el.classList.add('is-in');
    io.unobserve(e.target);
  }), { rootMargin: '0px 0px -10% 0px', threshold: 0.01 });
  document.querySelectorAll('[data-paste], [data-print], .scribble, .circled').forEach(el => {
    if (el.hasAttribute('data-paste')) {
      const sibs = [...el.parentElement.children].filter(s => s.hasAttribute('data-paste'));
      el.style.setProperty('--i', sibs.indexOf(el) % 6);
    }
    if (el.hasAttribute('data-print')) {
      el.parentElement.printChild = el;
      io.observe(el.parentElement);
    } else {
      io.observe(el);
    }
  });

  const layers = [...document.querySelectorAll('[data-depth]')];
  const scenes = [...document.querySelectorAll('[data-scene]')];
  const reels = [...document.querySelectorAll('.reel')].map(section => ({
    section,
    pin: section.querySelector('.reel-pin'),
    track: section.querySelector('.reel-track'),
    strip: section.querySelector('.build-grid'),
    dist: 0,
  }));
  const wide = matchMedia('(min-width: 900px)');
  const finePointer = matchMedia('(pointer: fine)').matches;
  let px = 0, py = 0, queued = false;

  function measureReels() {
    for (const r of reels) {
      r.strip.style.transform = '';
      if (!wide.matches) {
        r.section.classList.remove('is-reel');
        r.section.style.height = '';
        r.dist = 0;
        continue;
      }
      r.section.classList.add('is-reel');
      // travel far enough that the last frame clears the edge by the same margin the first one starts at
      const padLeft = parseFloat(getComputedStyle(r.track).paddingLeft) || 0;
      r.dist = Math.max(0, r.strip.offsetWidth + padLeft * 2 - r.track.clientWidth);
      r.section.style.height = `${r.pin.offsetHeight + r.dist}px`;
    }
  }

  function frame() {
    queued = false;
    const vh = innerHeight;
    for (const el of layers) {
      const d = parseFloat(el.dataset.depth) || 0;
      const box = el.parentElement.getBoundingClientRect();
      const fromCentre = box.top + box.height / 2 - vh / 2;
      el.style.transform = `translate3d(${(-px * d * 34).toFixed(1)}px, ${(-py * d * 26 - fromCentre * d * 0.08).toFixed(1)}px, 0)`;
    }
    for (const el of scenes) {
      const box = el.getBoundingClientRect();
      const p = Math.min(1, Math.max(0, (vh - box.top) / (vh + box.height)));
      el.style.setProperty('--p', p.toFixed(3));
    }
    for (const r of reels) {
      if (!r.dist) continue;
      const box = r.section.getBoundingClientRect();
      const travel = r.section.offsetHeight - r.pin.offsetHeight;
      const p = travel > 0 ? Math.min(1, Math.max(0, -box.top / travel)) : 0;
      r.strip.style.transform = `translate3d(${(-p * r.dist).toFixed(1)}px, 0, 0)`;
    }
  }
  const queue = () => { if (!queued) { queued = true; requestAnimationFrame(frame); } };

  if (finePointer) {
    addEventListener('pointermove', e => {
      px = e.clientX / innerWidth - 0.5;
      py = e.clientY / innerHeight - 0.5;
      queue();
    }, { passive: true });
  }
  addEventListener('scroll', queue, { passive: true });
  addEventListener('resize', () => { measureReels(); queue(); });
  reduce.addEventListener('change', () => location.reload());

  measureReels();
  document.fonts.ready.then(() => { measureReels(); queue(); });
  queue();
})();
